#include <SDL_image.h>
#include <Components/ParentComponent.hpp>
#include "RenderWrapper.hpp"

#include <EngineManagers/InputManager.hpp>

#include "../includes/SystemManager.hpp"
#include "../ConfigSingleton.hpp"
#include "../includes/ComponentStore.hpp"
#include "../../outfacingInterfaces/EngineManagers/SceneManager.hpp"

#pragma region Initialize

RenderWrapper::RenderWrapper() : renderer(nullptr, nullptr), renderTexture(nullptr, nullptr) {
    Initialize();
}

RenderWrapper::~RenderWrapper() {
    Cleanup();
}

void RenderWrapper::handleEvents() {
    SDL_Event event;
    SDL_Event lastMouseMotionEvent{};
    SDL_Event lastMouseButtonDownEvent{};
    SDL_Event lastMouseButtonUpEvent{};
    SDL_Event lastKeyDownEvent{};
    SDL_Event lastKeyUpEvent{};
    SDL_Event resizeEvent{};
    bool isMaximised = false;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                ConfigSingleton::getInstance().toggleIsRunning();
                break;
            }
            case SDL_MOUSEMOTION: {
                lastMouseMotionEvent = event;
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                lastMouseButtonDownEvent = event;
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                lastMouseButtonUpEvent = event;
                break;
            }
            case SDL_KEYDOWN: {
                lastKeyDownEvent = event;
                break;
            }
            case SDL_KEYUP: {
                lastKeyUpEvent = event;
                break;
            }
            case SDL_WINDOWEVENT: {
                if (event.window.event == SDL_WINDOWEVENT_MAXIMIZED)
                    isMaximised = true;
                else if (event.window.event == SDL_WINDOWEVENT_RESTORED)
                    isMaximised = false;
                else if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    resizeEvent = event;

                break;
            }
        }
    }

    if (resizeEvent.type != 0) {
        int width, height;
        SDL_GetWindowSize(window.get(), &width, &height);
        if (!isMaximised) {
            auto aspectRatio = ConfigSingleton::getInstance().getAspectRatio();
            if (width / aspectRatio > height) {
                width = static_cast<int>(height * aspectRatio);
            } else {
                height = static_cast<int>(width / aspectRatio);
            }
            SDL_SetWindowSize(window.get(), width, height);
        }

        ConfigSingleton::getInstance().setWindowSize(Vector2(width, height));
    }

    if (lastMouseMotionEvent.type != 0) {
        InputManager::GetInstance().SetMousePosition(
                Vector2(static_cast<float>(lastMouseMotionEvent.motion.x),
                        static_cast<float>(lastMouseMotionEvent.motion.y)));
    }
    if (lastMouseButtonDownEvent.type != 0) {
        InputManager::GetInstance().SetMousePressed(lastMouseButtonDownEvent.button.button);
    }
    if (lastMouseButtonUpEvent.type != 0) {
        InputManager::GetInstance().SetMouseReleased(lastMouseButtonUpEvent.button.button);
    }
    if (lastKeyDownEvent.type != 0) {
        InputManager::GetInstance().SetKeyPressed(lastKeyDownEvent.key.keysym.sym);
    }
    if (lastKeyUpEvent.type != 0) {
        InputManager::GetInstance().SetKeyReleased(lastKeyUpEvent.key.keysym.sym);
    }

    auto configFullScreen = ConfigSingleton::getInstance().isFullscreen();
    if (configFullScreen != fullscreen) {
        fullscreen = configFullScreen;
        if (fullscreen)
            SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
        else
            SDL_SetWindowFullscreen(window.get(), 0);
    }
}

bool RenderWrapper::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL2 initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cerr << "SDL2_image initialization failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
    }

    std::unique_ptr<SDL_Window, SDLWindowDeleter> tempWindow(
            SDL_CreateWindow(ConfigSingleton::getInstance().getWindowTitle().c_str(),
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, ConfigSingleton::getInstance().getWindowSize().getX(),
                             ConfigSingleton::getInstance().getWindowSize().getY(),
                             SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));
    window = std::move(tempWindow);

    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        Cleanup();
        return false;
    }

    renderer = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>(
            SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED),
            [](SDL_Renderer *r) { SDL_DestroyRenderer(r); });

    if (renderer == nullptr) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        Logger::GetInstance().Shutdown();
        SDL_DestroyWindow(window.get());
        Cleanup();
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
    renderTexture = std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>(
            SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                              ConfigSingleton::getInstance().getWindowSize().getX(),
                              ConfigSingleton::getInstance().getWindowSize().getY()),
            [](SDL_Texture *t) { SDL_DestroyTexture(t); });

    SDL_SetRenderTarget(renderer.get(), renderTexture.get());
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255); // RGBA format
    SDL_RenderClear(renderer.get());

    return true;
}

void RenderWrapper::Cleanup() {
    // Perform cleanup as necessary
    Logger::GetInstance().Shutdown();
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
}

void RenderWrapper::cleanCache() {
    cameraTextures.clear();
    textures.clear();
}

#pragma endregion

#pragma region RenderElements

void
RenderWrapper::RenderTileMap(const CameraComponent &cameraComponent, const TransformComponent &cameraTransformComponent,
                             const TileMapComponent &tileMapComponent, const TransformComponent &transformComponent) {
    auto &cameraPosition = cameraTransformComponent.position;
    auto &cameraSize = cameraComponent.size;
    auto tileMapPosition = SceneManager::getWorldPosition(transformComponent);
    auto tileMapScale = SceneManager::getWorldScale(transformComponent);

    auto &tileMap = tileMapComponent.tileMap;
    size_t maxWidth = 0;
    for (auto &row: tileMap) {
        maxWidth = std::max(maxWidth, row.size());
    }

    auto tileMapSize = Vector2(maxWidth * tileMapComponent.tileSize->getX(),
                               tileMap.size() * tileMapComponent.tileSize->getY());

    auto sizeX = tileMapSize.getX() * tileMapScale.getX();
    auto sizeY = tileMapSize.getY() * tileMapScale.getY();

    if (tileMapPosition.getX() + sizeX / 2 < cameraPosition->getX() - cameraSize->getX() / 2 ||
        tileMapPosition.getX() - sizeX / 2 > cameraPosition->getX() + cameraSize->getX() / 2 ||
        tileMapPosition.getY() + sizeY / 2 < cameraPosition->getY() - cameraSize->getY() / 2 ||
        tileMapPosition.getY() - sizeY / 2 > cameraPosition->getY() + cameraSize->getY() / 2)
        return;

    size_t xTileAmount = ceil(cameraSize->getX() / (tileMapComponent.tileSize->getX() * tileMapScale.getX())) + 1;
    size_t yTileAmount = ceil(cameraSize->getY() / (tileMapComponent.tileSize->getY() * tileMapScale.getY())) + 1;

    auto leftMostCameraPosition = cameraPosition->getX() - cameraSize->getX() / 2;
    auto topMostCameraPosition = cameraPosition->getY() - cameraSize->getY() / 2;
    auto leftMostTileMapPosition = tileMapPosition.getX() - sizeX / 2;
    auto topMostTileMapPosition = tileMapPosition.getY() - sizeY / 2;

    auto xDifference = leftMostCameraPosition - leftMostTileMapPosition;
    auto yDifference = topMostCameraPosition - topMostTileMapPosition;

    size_t xStartIndex = xDifference > 0
                         ? floor(
                    (xDifference) / (tileMapComponent.tileSize->getX() * tileMapScale.getX()))
                         : 0;
    size_t yStartIndex = yDifference > 0
                         ? floor(
                    (yDifference) / (tileMapComponent.tileSize->getY() * tileMapScale.getY()))
                         : 0;

    size_t yEndIndex = std::min(yStartIndex + yTileAmount, tileMap.size());


    if (textures.find(tileMapComponent.tileMapPath) == textures.end())
        textures.insert(std::make_pair(tileMapComponent.tileMapPath, GetSpriteTexture(tileMapComponent.tileMapPath)));
    auto texture = textures.find(tileMapComponent.tileMapPath);
    for (size_t y = yStartIndex; y < yEndIndex; ++y) {
        size_t xEndIndex = std::min(xStartIndex + xTileAmount, tileMap[y].size());
        for (size_t x = xStartIndex; x < xEndIndex; ++x) {
            auto &tile = tileMap[y][x];
            if (!tile)
                continue;
            SDL_Rect srcRect;
            int spriteWidth = tileMapComponent.tileSize->getX();
            int spriteHeight = tileMapComponent.tileSize->getY();
            srcRect.x = (tileMap[y][x]->getX() * spriteWidth) +
                        (tileMapComponent.margin * tileMap[y][x]->getX());
            srcRect.y = (tileMap[y][x]->getY() * spriteHeight) + (tileMapComponent.margin * tileMap[y][x]->getY());
            srcRect.w = spriteWidth;
            srcRect.h = spriteHeight;

            auto width = tileMapComponent.tileSize->getX() * tileMapScale.getX();
            auto height = tileMapComponent.tileSize->getY() * tileMapScale.getY();

            SDL_Rect destRect = {
                    static_cast<int>(tileMapPosition.getX() - cameraTransformComponent.position->getX() +
                                     cameraComponent.size->getX() / 2 - sizeX / 2 + x * width),
                    static_cast<int>(tileMapPosition.getY() - cameraTransformComponent.position->getY() +
                                     cameraComponent.size->getY() / 2 - sizeY / 2 + y * height),
                    static_cast<int>(width),
                    static_cast<int>(height)
            };

            SDL_RenderCopy(renderer.get(), texture->second.get(), &srcRect, &destRect);
        }
    }
}

void
RenderWrapper::RenderSprite(const CameraComponent &cameraComponent, const TransformComponent &cameraTransformComponent,
                            const SpriteComponent &spriteComponent, const TransformComponent &transformComponent) {
    auto &cameraPosition = cameraTransformComponent.position;
    auto &cameraSize = cameraComponent.size;
    auto spritePosition = SceneManager::getWorldPosition(transformComponent);
    auto &spriteSize = spriteComponent.spriteSize;
    auto spriteScale = SceneManager::getWorldScale(transformComponent);
    auto sizeX = spriteSize->getX() * spriteScale.getX();
    auto sizeY = spriteSize->getY() * spriteScale.getY();
    auto spriteRotation = SceneManager::getWorldRotation(transformComponent);

    if (spritePosition.getX() + sizeX / 2 < cameraPosition->getX() - cameraSize->getX() / 2 ||
        spritePosition.getX() - sizeX / 2 > cameraPosition->getX() + cameraSize->getX() / 2 ||
        spritePosition.getY() + sizeY / 2 < cameraPosition->getY() - cameraSize->getY() / 2 ||
        spritePosition.getY() - sizeY / 2 > cameraPosition->getY() + cameraSize->getY() / 2)
        return;

    if (textures.find(spriteComponent.spritePath) == textures.end())
        textures.insert(std::make_pair(spriteComponent.spritePath, GetSpriteTexture(spriteComponent.spritePath)));
    auto texture = textures.find(spriteComponent.spritePath);

    //Fill in a rectangle for the current sprite IN
    SDL_Rect srcRect;
    int spriteWidth = spriteComponent.spriteSize->getX();
    int spriteHeight = spriteComponent.spriteSize->getY();
    srcRect.x = (spriteComponent.tileOffset->getX() * spriteWidth) +
                (spriteComponent.margin * spriteComponent.tileOffset->getX());
    srcRect.y = (spriteComponent.tileOffset->getY() * spriteHeight) +
                (spriteComponent.margin * spriteComponent.tileOffset->getY());
    srcRect.w = spriteWidth;
    srcRect.h = spriteHeight;


    auto width = spriteComponent.spriteSize->getX() * spriteScale.getX();
    auto height = spriteComponent.spriteSize->getY() * spriteScale.getY();
    //Create a rectangle were the sprite needs to be rendered on to
    SDL_Rect destRect = {
            static_cast<int>(spritePosition.getX() - cameraTransformComponent.position->getX() +
                             cameraComponent.size->getX() / 2 - width / 2),
            static_cast<int>(spritePosition.getY() - cameraTransformComponent.position->getY() +
                             cameraComponent.size->getY() / 2 - height / 2),
            static_cast<int>(width),
            static_cast<int>(height)
    };

    render(texture->second.get(), &srcRect, &destRect, spriteRotation, spriteComponent.flipX, spriteComponent.flipY);
}

void
RenderWrapper::RenderText(const CameraComponent &cameraComponent, const TransformComponent &cameraTransformComponent,
                          const TextComponent &textComponent, const TransformComponent &transformComponent) {
    SDL_Color sdlColor = {
            static_cast<Uint8>(textComponent.color->r),
            static_cast<Uint8>(textComponent.color->g),
            static_cast<Uint8>(textComponent.color->b),
            static_cast<Uint8>(textComponent.color->a)
    };

    TTF_Font *font = nullptr;
    const std::string &fontPath = textComponent.fontPath;
    int fontSize = textComponent.fontSize;

    auto &sizeMap = fontCache[fontPath];
    if (sizeMap.count(fontSize) != 0) {
        font = sizeMap[fontSize];
    } else {
        font = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (!font) {
            std::string baseFontPath = ConfigSingleton::getInstance().getBaseAssetPath() + "Fonts/Default.ttf";
            font = TTF_OpenFont(baseFontPath.c_str(), fontSize);
        }
        sizeMap[fontSize] = font;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font, textComponent.text.c_str(), sdlColor);

    if (!surface) {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
    }

    auto &cameraPosition = cameraTransformComponent.position;
    auto &cameraSize = cameraComponent.size;
    auto textPosition = SceneManager::getWorldPosition(transformComponent);
    auto textRotation = SceneManager::getWorldRotation(transformComponent);
    auto sizeX = surface->w;
    auto sizeY = surface->h;

    if (textPosition.getX() + sizeX / 2 < cameraPosition->getX() - cameraSize->getX() / 2 ||
        textPosition.getX() - sizeX / 2 > cameraPosition->getX() + cameraSize->getX() / 2 ||
        textPosition.getY() + sizeY / 2 < cameraPosition->getY() - cameraSize->getY() / 2 ||
        textPosition.getY() - sizeY / 2 > cameraPosition->getY() + cameraSize->getY() / 2)
        return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }

    SDL_Rect rect = {
            static_cast<int>(textPosition.getX() - cameraTransformComponent.position->getX() +
                             cameraComponent.size->getX() / 2 - sizeX / 2),
            static_cast<int>(textPosition.getY() - cameraTransformComponent.position->getY() +
                             cameraComponent.size->getY() / 2 - sizeY / 2),
            static_cast<int>(sizeX),
            static_cast<int>(sizeY)
    };

    render(texture, nullptr, &rect, textRotation, textComponent.flipX, textComponent.flipY);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void RenderWrapper::RenderRectangle(const CameraComponent &cameraComponent,
                                    const TransformComponent &cameraTransformComponent,
                                    const RectangleComponent &rectangleComponent,
                                    const TransformComponent &transformComponent) {
    auto &cameraPosition = cameraTransformComponent.position;
    auto &cameraSize = cameraComponent.size;
    auto rectanglePosition = SceneManager::getWorldPosition(transformComponent);
    auto rectangleScale = SceneManager::getWorldScale(transformComponent);
    auto &rectangleSize = rectangleComponent.size;
    auto sizeX = rectangleSize->getX() * rectangleScale.getX();
    auto sizeY = rectangleSize->getY() * rectangleScale.getY();

    auto renderInstance = renderer.get();

    auto rectangleRotation = SceneManager::getWorldRotation(transformComponent);

    if (rectanglePosition.getX() + sizeX / 2 < cameraPosition->getX() - cameraSize->getX() / 2 ||
        rectanglePosition.getX() - sizeX / 2 > cameraPosition->getX() + cameraSize->getX() / 2 ||
        rectanglePosition.getY() + sizeY / 2 < cameraPosition->getY() - cameraSize->getY() / 2 ||
        rectanglePosition.getY() - sizeY / 2 > cameraPosition->getY() + cameraSize->getY() / 2)
        return;

    SDL_Rect rect = {
            static_cast<int>(rectanglePosition.getX() - cameraTransformComponent.position->getX() +
                             cameraComponent.size->getX() / 2 - sizeX / 2),
            static_cast<int>(rectanglePosition.getY() - cameraTransformComponent.position->getY() +
                             cameraComponent.size->getY() / 2 - sizeY / 2),
            static_cast<int>(sizeX), static_cast<int>(sizeY)
    };

    SDL_Texture *rectangleTexture = SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_RGBA8888,
                                                      SDL_TEXTUREACCESS_TARGET,
                                                      sizeX, sizeY);
    SDL_SetRenderTarget(renderInstance, rectangleTexture);

    SDL_SetRenderDrawColor(renderInstance, rectangleComponent.fill->r, rectangleComponent.fill->g,
                           rectangleComponent.fill->b, rectangleComponent.fill->a);
    SDL_RenderClear(renderInstance);
    auto &texturePair = GetCameraTexturePair(cameraComponent);

    SDL_SetRenderTarget(renderInstance, texturePair.second.get());

    render(rectangleTexture, nullptr, &rect, rectangleRotation, rectangleComponent.flipX, rectangleComponent.flipY);

    SDL_DestroyTexture(rectangleTexture);
}

void
RenderWrapper::RenderUiTileMap(const TileMapComponent &tileMapComponent, const TransformComponent &transformComponent) {
    Logger::GetInstance().Error("Tilemap cannot be rendered in UI");
}

void
RenderWrapper::RenderUiSprite(const SpriteComponent &spriteComponent, const TransformComponent &transformComponent) {
    if (textures.find(spriteComponent.spritePath) == textures.end())
        textures.insert(std::make_pair(spriteComponent.spritePath, GetSpriteTexture(spriteComponent.spritePath)));
    auto texture = textures.find(spriteComponent.spritePath);

    auto spritePosition = SceneManager::getWorldPosition(transformComponent);
    auto spriteScale = SceneManager::getWorldScale(transformComponent);
    auto spriteRotation = SceneManager::getWorldRotation(transformComponent);
    auto sizeX = spriteComponent.spriteSize->getX() * spriteScale.getX();
    auto sizeY = spriteComponent.spriteSize->getY() * spriteScale.getY();
    //Fill in a rectangle for the current sprite IN
    SDL_Rect srcRect;
    int spriteWidth = spriteComponent.spriteSize->getX();
    int spriteHeight = spriteComponent.spriteSize->getY();
    srcRect.x = (spriteComponent.tileOffset->getX() * spriteWidth) +
                (spriteComponent.margin * spriteComponent.tileOffset->getX());
    srcRect.y = (spriteComponent.tileOffset->getY() * spriteHeight) +
                (spriteComponent.margin * spriteComponent.tileOffset->getY());
    srcRect.w = spriteWidth;
    srcRect.h = spriteHeight;

    //Create a rectangle were the sprite needs to be rendered on to
    SDL_Rect destRect = {
            static_cast<int>(spritePosition.getX()), static_cast<int>(spritePosition.getY()),
            static_cast<int>(sizeX), static_cast<int>(sizeY)
    };

    render(texture->second.get(), &srcRect, &destRect, spriteRotation, spriteComponent.flipX, spriteComponent.flipY);
}

void RenderWrapper::RenderUiText(const TextComponent &textComponent, const TransformComponent &transformComponent) {
    SDL_Color sdlColor = {
            static_cast<Uint8>(textComponent.color->r),
            static_cast<Uint8>(textComponent.color->g),
            static_cast<Uint8>(textComponent.color->b),
            static_cast<Uint8>(textComponent.color->a)
    };

    auto textPosition = SceneManager::getWorldPosition(transformComponent);
    auto textRotation = SceneManager::getWorldRotation(transformComponent);

    TTF_Font *font = nullptr;
    const std::string &fontPath = textComponent.fontPath;
    int fontSize = textComponent.fontSize;

    auto &sizeMap = fontCache[fontPath];
    if (sizeMap.count(fontSize) != 0) {
        font = sizeMap[fontSize];
    } else {
        font = TTF_OpenFont(fontPath.c_str(), fontSize);
        if (!font) {
            std::string baseFontPath = ConfigSingleton::getInstance().getBaseAssetPath() + "Fonts/Default.ttf";
            font = TTF_OpenFont(baseFontPath.c_str(), fontSize);
        }
        sizeMap[fontSize] = font;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font, textComponent.text.c_str(), sdlColor);

    if (!surface) {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }

    SDL_Rect rect = {
            static_cast<int>(textPosition.getX()), static_cast<int>(textPosition.getY()), surface->w,
            surface->h
    };

    render(texture, nullptr, &rect, textRotation, textComponent.flipX, textComponent.flipY);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


void RenderWrapper::RenderUiRectangle(const RectangleComponent &rectangleComponent,
                                      const TransformComponent &transformComponent) {
    auto rectanglePosition = SceneManager::getWorldPosition(transformComponent);
    auto rectangleScale = SceneManager::getWorldScale(transformComponent);
    auto rectangleRotation = SceneManager::getWorldRotation(transformComponent);
    auto renderInstance = renderer.get();

    SDL_Rect rect = {
            static_cast<int>(rectanglePosition.getX()),
            static_cast<int>(rectanglePosition.getY()),
            static_cast<int>(rectangleComponent.size->getX() * rectangleScale.getX()),
            static_cast<int>(rectangleComponent.size->getY() * rectangleScale.getY())
    };

    SDL_Texture *rectangleTexture = SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_RGBA8888,
                                                      SDL_TEXTUREACCESS_TARGET, rectangleComponent.size->getX(),
                                                      rectangleComponent.size->getY());

    SDL_SetRenderTarget(renderInstance, rectangleTexture);

    SDL_SetRenderDrawColor(renderInstance, rectangleComponent.fill->r, rectangleComponent.fill->g,
                           rectangleComponent.fill->b, rectangleComponent.fill->a);
    SDL_RenderClear(renderInstance);

    SDL_SetRenderTarget(renderInstance, renderTexture.get());
    render(rectangleTexture, nullptr, &rect, rectangleRotation, rectangleComponent.flipX, rectangleComponent.flipY);

    SDL_DestroyTexture(rectangleTexture);
}


#pragma endregion

#pragma region RenderDebugElements

void RenderWrapper::RenderBoxCollision(const CameraComponent &cameraComponent,
                                       const TransformComponent &cameraTransformComponent,
                                       const BoxCollisionComponent &boxCollisionComponent,
                                       const TransformComponent &transformComponent) {
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    auto &cameraPosition = cameraTransformComponent.position;
    auto &cameraSize = cameraComponent.size;
    auto boxPosition = SceneManager::getWorldPosition(transformComponent) + *boxCollisionComponent.offset;
    auto boxScale = SceneManager::getWorldScale(transformComponent);
    auto &size = boxCollisionComponent.size;
    auto sizeX = size->getX() * boxScale.getX();
    auto sizeY = size->getY() * boxScale.getY();

    if (boxPosition.getX() + sizeX / 2 < cameraPosition->getX() - cameraSize->getX() / 2 ||
        boxPosition.getX() - sizeX / 2 > cameraPosition->getX() + cameraSize->getX() / 2 ||
        boxPosition.getY() + sizeY / 2 < cameraPosition->getY() - cameraSize->getY() / 2 ||
        boxPosition.getY() - sizeY / 2 > cameraPosition->getY() + cameraSize->getY() / 2)
        return;

    SDL_Rect squareRect = {
            static_cast<int>(boxPosition.getX() - cameraTransformComponent.position->getX() +
                             cameraComponent.size->getX() / 2 - sizeX / 2),
            static_cast<int>(boxPosition.getY() - cameraTransformComponent.position->getY() +
                             cameraComponent.size->getY() / 2 - sizeY / 2),
            static_cast<int>(sizeX),
            static_cast<int>(sizeY)
    };

    // Render the button background (you can customize this part)
    SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer.get(), &squareRect);
#endif
}

void RenderWrapper::RenderGraph(const CameraComponent &cameraComponent,
                                const TransformComponent &cameraTransformComponent,
                                const GraphComponent &graphComponent,
                                const TransformComponent &graphTransformComponent) {
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    for (auto &graphNode: graphComponent.graph_) {
        auto &cameraPosition = cameraTransformComponent.position;
        auto &cameraSize = cameraComponent.size;
        auto boxPosition = graphNode->getPosition() + *graphTransformComponent.position;
        auto sizeX = 5.0;
        auto sizeY = 5.0;

        if (boxPosition.getX() + sizeX / 2 < cameraPosition->getX() - cameraSize->getX() / 2 ||
            boxPosition.getX() - sizeX / 2 > cameraPosition->getX() + cameraSize->getX() / 2 ||
            boxPosition.getY() + sizeY / 2 < cameraPosition->getY() - cameraSize->getY() / 2 ||
            boxPosition.getY() - sizeY / 2 > cameraPosition->getY() + cameraSize->getY() / 2)
            continue;

        SDL_Rect squareRect = {
                static_cast<int>(boxPosition.getX() - cameraTransformComponent.position->getX() +
                                 cameraComponent.size->getX() / 2 - sizeX / 2),
                static_cast<int>(boxPosition.getY() - cameraTransformComponent.position->getY() +
                                 cameraComponent.size->getY() / 2 - sizeY / 2),
                static_cast<int>(sizeX),
                static_cast<int>(sizeY)
        };
        if (graphNode->isVisited()) {
            SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer.get(), 0, 255, 0, 255);
        }
        SDL_RenderDrawRect(renderer.get(), &squareRect);
    }
#endif
}

void RenderWrapper::RenderUiBoxCollision(const BoxCollisionComponent &boxCollisionComponent,
                                         const TransformComponent &transformComponent) {
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    auto worldPosition = SceneManager::getWorldPosition(transformComponent);
    auto worldScale = SceneManager::getWorldScale(transformComponent);

    SDL_Rect squareRect = {
            static_cast<int>(worldPosition.getX()),
            static_cast<int>(worldPosition.getY()),
            static_cast<int>(boxCollisionComponent.size->getX() * worldScale.getX()),
            static_cast<int>(boxCollisionComponent.size->getY() * worldScale.getY())
    };

    SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer.get(), &squareRect);
#endif
}


#pragma endregion

#pragma region Rendering

void RenderWrapper::RenderFrame() {
    auto renderInstance = renderer.get();
    auto renderTextureInstance = renderTexture.get();
    SDL_SetRenderTarget(renderInstance, nullptr);
    SDL_SetRenderDrawColor(renderInstance, 0, 0, 0, 255); // RGBA format
    SDL_RenderClear(renderInstance);
    SDL_RenderCopy(renderInstance, renderTextureInstance, nullptr, nullptr);
    SDL_RenderPresent(renderInstance);
    SDL_SetRenderTarget(renderInstance, renderTextureInstance);
    SDL_SetRenderDrawColor(renderInstance, 0, 0, 0, 255); // RGBA format
    SDL_RenderClear(renderInstance);
}

void RenderWrapper::RenderCamera(const CameraComponent &cameraComponent) {
    auto &backgroundColor = cameraComponent.backgroundColor;
    auto renderInstance = renderer.get();
    SDL_SetRenderDrawColor(renderInstance, backgroundColor->r, backgroundColor->g, backgroundColor->b,
                           backgroundColor->a); // RGBA format

    auto &texturePair = GetCameraTexturePair(cameraComponent);

    SDL_SetRenderTarget(renderInstance, texturePair.second.get());
    SDL_RenderClear(renderInstance);
}

void RenderWrapper::render(SDL_Texture *texture, SDL_Rect *srcrect, SDL_Rect *dstrect, float rotation, const bool flipX,
                           const bool flipY) const {
    int centerX = dstrect->w / 2;
    int centerY = dstrect->h / 2;
    SDL_Point rotationCenter = {centerX, centerY};

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (flipX)
        flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_HORIZONTAL);
    if (flipY)
        flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);

    SDL_RenderCopyEx(renderer.get(), texture, srcrect, dstrect, rotation, &rotationCenter, flip);
}

void RenderWrapper::RenderToMainTexture() {
    SDL_SetRenderTarget(renderer.get(), renderTexture.get());

    for (auto &cameraTexture: cameraTextures) {
        auto transformComp = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(cameraTexture.first);

        render(cameraTexture.second.second.get(), nullptr, &cameraTexture.second.first, transformComp.rotation, false,
               false);
    }
}

#pragma endregion

#pragma region Helpers

std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> RenderWrapper::GetSpriteTexture(std::string filePath) {
    // Get the file extension
    auto newPath = ConfigSingleton::getInstance().getBaseAssetPath() + filePath;

    size_t dotPos = newPath.find_last_of('.');
    if (dotPos == std::string::npos) {
        std::cerr << "Error: Invalid file path (no file extension)" << std::endl;
        return std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(nullptr, &SDL_DestroyTexture);
    }

    std::string extension = newPath.substr(dotPos + 1);

    if (extension == "bmp") {
        SDL_Surface *surface = SDL_LoadBMP(newPath.c_str());
        if (surface) {
            std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> bmpTexture(
                    SDL_CreateTextureFromSurface(renderer.get(), surface), &SDL_DestroyTexture);
            SDL_FreeSurface(surface);
            return std::move(bmpTexture);
        } else {
            std::cerr << "Error: Failed to load BMP file: " << SDL_GetError() << std::endl;
        }
    } else if (extension == "png") {
        SDL_Texture *pngTexture = IMG_LoadTexture(renderer.get(), newPath.c_str());
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture(pngTexture, &SDL_DestroyTexture);
        if (pngTexture) {
            return std::move(texture);
        } else {
            std::cerr << "Error: Failed to load PNG file: " << IMG_GetError() << std::endl;
        }
    } else {
        std::cerr << "Error: Unsupported file type: " << extension << std::endl;
    }

    return std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(nullptr, &SDL_DestroyTexture);
}

std::pair<SDL_Rect, std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>> &
RenderWrapper::GetCameraTexturePair(const CameraComponent &cameraComponent) {
    auto cameraTexture = cameraTextures.find(cameraComponent.entityId);
    if (cameraTexture == cameraTextures.end()) {
        auto width = cameraComponent.size->getX();
        auto height = cameraComponent.size->getY();
        auto xPosition = cameraComponent.onScreenPosition->getX() - width / 2;
        auto yPosition = cameraComponent.onScreenPosition->getY() - height / 2;

        SDL_Rect rect = {
                static_cast<int>(xPosition), static_cast<int>(yPosition), static_cast<int>(width),
                static_cast<int>(height)
        };
        cameraTextures.insert(
                std::make_pair(cameraComponent.entityId,
                               std::make_pair(rect, std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>(
                                       SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_RGBA8888,
                                                         SDL_TEXTUREACCESS_TARGET, width, height),
                                       [](SDL_Texture *t) { SDL_DestroyTexture(t); }))));
    }
    cameraTexture = cameraTextures.find(cameraComponent.entityId);
    return cameraTexture->second;
}

void RenderWrapper::RenderCircleCollision(const CameraComponent &cameraComponent,
                                          const TransformComponent &cameraTransformComponent,
                                          const CircleCollisionComponent &circleCollisionComponent,
                                          const TransformComponent &transformComponent) {
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    auto &cameraPosition = cameraTransformComponent.position;
    auto &cameraSize = cameraComponent.size;
    auto circlePosition = SceneManager::getWorldPosition(transformComponent) + *circleCollisionComponent.offset;
    auto circleScale = SceneManager::getWorldScale(transformComponent);
    auto circleRadius = circleCollisionComponent.radius * circleScale.getX();

    if (circlePosition.getX() + circleRadius < cameraPosition->getX() - cameraSize->getX() / 2 ||
        circlePosition.getX() - circleRadius > cameraPosition->getX() + cameraSize->getX() / 2 ||
        circlePosition.getY() + circleRadius < cameraPosition->getY() - cameraSize->getY() / 2 ||
        circlePosition.getY() - circleRadius > cameraPosition->getY() + cameraSize->getY() / 2)
        return;

    if (textures.find("Resources/Circle.png") == textures.end())
        textures.insert(std::make_pair("Resources/Circle.png", GetSpriteTexture("Resources/Circle.png")));
    auto texture = textures.find("Resources/Circle.png");

    SDL_Rect srcRect{0, 0, 512, 512};
    SDL_Rect destRect = {
            static_cast<int>(circlePosition.getX() - cameraTransformComponent.position->getX() +
                             cameraComponent.size->getX() / 2 - circleRadius),
            static_cast<int>(circlePosition.getY() - cameraTransformComponent.position->getY() +
                             cameraComponent.size->getY() / 2 - circleRadius),
            static_cast<int>(circleRadius * 2),
            static_cast<int>(circleRadius * 2)
    };

    render(texture->second.get(), &srcRect, &destRect, 0, false, false);
#endif
}

void RenderWrapper::RenderUiCircleCollision(const CircleCollisionComponent &circleCollisionComponent,
                                            const TransformComponent &transformComponent) {
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG

    auto worldPosition = SceneManager::getWorldPosition(transformComponent);
    auto worldScale = SceneManager::getWorldScale(transformComponent);
    auto circleRadius = circleCollisionComponent.radius * worldScale.getX();

    if (textures.find("Resources/Circle.png") == textures.end())
        textures.insert(std::make_pair("Resources/Circle.png", GetSpriteTexture("Resources/Circle.png")));
    auto texture = textures.find("Resources/Circle.png");

    SDL_Rect srcRect{0, 0, 512, 512};
    SDL_Rect destRect = {
            static_cast<int>(worldPosition.getX() - circleRadius),
            static_cast<int>(worldPosition.getY() - circleRadius),
            static_cast<int>(circleRadius * 2),
            static_cast<int>(circleRadius * 2)
    };

    render(texture->second.get(), &srcRect, &destRect, 0, false, false);

#endif
}

#pragma endregion
