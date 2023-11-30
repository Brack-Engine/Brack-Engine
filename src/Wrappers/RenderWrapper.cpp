#include <SDL_image.h>
#include "RenderWrapper.hpp"
#include "../includes/SystemManager.hpp"
#include "../ConfigSingleton.hpp"
#include "../Logger.hpp"

RenderWrapper::RenderWrapper() : renderer(nullptr, nullptr), renderTexture(nullptr, nullptr) {
    Initialize();
}

RenderWrapper::~RenderWrapper() {
    Cleanup();
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

    // Create a window.
    // You can customize the window size, title, and other settings as needed.
    // For simplicity, this example creates a 800x600 window.
    std::unique_ptr<SDL_Window, SDLWindowDeleter> tempWindow(
            SDL_CreateWindow(ConfigSingleton::GetInstance().GetWindowTitle().c_str(),
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, ConfigSingleton::GetInstance().GetWindowSize().getX(),
                             ConfigSingleton::GetInstance().GetWindowSize().getY(), SDL_WINDOW_SHOWN)
    );
    window = std::move(tempWindow);

    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        Cleanup();
        return false;
    }

    // Perform additional initialization as needed (e.g., renderer setup, resource loading).
    renderer = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>(
            SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED),
            [](SDL_Renderer *r) { SDL_DestroyRenderer(r); }
    );

    if (renderer == nullptr) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        Logger::GetInstance().shutdown();
        SDL_DestroyWindow(window.get());
        Cleanup();
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
    renderTexture = std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>(
            SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                              ConfigSingleton::GetInstance().GetWindowSize().getX(),
                              ConfigSingleton::GetInstance().GetWindowSize().getY()),
            [](SDL_Texture *t) { SDL_DestroyTexture(t); }
    );

    SDL_SetRenderTarget(renderer.get(), renderTexture.get());
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255); // RGBA format
    SDL_RenderClear(renderer.get());

    return true;
}

void RenderWrapper::Cleanup() {
    // Perform cleanup as necessary
    Logger::GetInstance().shutdown();
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
}

void RenderWrapper::RenderFrame() {
    SDL_SetRenderTarget(renderer.get(), nullptr);
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255); // RGBA format
    SDL_RenderClear(renderer.get());
    SDL_RenderCopy(renderer.get(),
                   renderTexture.get(),
                   nullptr,
                   nullptr);
    SDL_RenderPresent(renderer.get());
    SDL_SetRenderTarget(renderer.get(), renderTexture.get());
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255); // RGBA format
    SDL_RenderClear(renderer.get());
}

std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> RenderWrapper::GetSpriteTexture(std::string filePath) {
    // Get the file extension
    auto newPath = ConfigSingleton::GetInstance().GetBaseAssetPath() + filePath;

    size_t dotPos = newPath.find_last_of('.');
    if (dotPos == std::string::npos) {
        std::cerr << "error: Invalid file path (no file extension)" << std::endl;
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
            std::cerr << "error: Failed to load BMP file: " << SDL_GetError() << std::endl;
        }
    } else if (extension == "png") {
        SDL_Texture *pngTexture = IMG_LoadTexture(renderer.get(), newPath.c_str());
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture(pngTexture, &SDL_DestroyTexture);
        if (pngTexture) {
            return std::move(texture);
        } else {
            std::cerr << "error: Failed to load PNG file: " << IMG_GetError() << std::endl;
        }
    } else {
        std::cerr << "error: Unsupported file type: " << extension << std::endl;
    }

    return std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(nullptr, &SDL_DestroyTexture);
}

void RenderWrapper::RenderCamera(const CameraComponent &cameraComponent, const TransformComponent &transformComponent) {
    auto &backgroundColor = cameraComponent.backgroundColor;
    SDL_SetRenderDrawColor(renderer.get(), backgroundColor->r, backgroundColor->g, backgroundColor->b,
                           backgroundColor->a); // RGBA format


    auto &texturePair = GetCameraTexturePair(cameraComponent, transformComponent);

    SDL_SetRenderTarget(renderer.get(), texturePair.second.get());
    SDL_RenderClear(renderer.get());
}

std::pair<SDL_Rect, std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>> &
RenderWrapper::GetCameraTexturePair(const CameraComponent &cameraComponent,
                                    const TransformComponent &transformComponent) {
    auto cameraTexture = cameraTextures.find(cameraComponent.entityID);
    if (cameraTexture == cameraTextures.end()) {
        auto width = cameraComponent.size->getX();
        auto height = cameraComponent.size->getY();
        auto xPosition = cameraComponent.onScreenPosition->getX() - width / 2;
        auto yPosition = cameraComponent.onScreenPosition->getY() - height / 2;

        SDL_Rect rect = {static_cast<int>(xPosition), static_cast<int>(yPosition), static_cast<int>(width),
                         static_cast<int>(height)};
        cameraTextures.insert(
                std::make_pair(cameraComponent.entityID,
                               std::make_pair(rect, std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>(
                                       SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_RGBA8888,
                                                         SDL_TEXTUREACCESS_TARGET,
                                                         width,
                                                         height),
                                       [](SDL_Texture *t) { SDL_DestroyTexture(t); }))));
    }
    cameraTexture = cameraTextures.find(cameraComponent.entityID);
    return cameraTexture->second;
}

void RenderWrapper::RenderCircleCollision(const CameraComponent &cameraComponent,
                                          const TransformComponent &cameraTransformComponent,
                                          const CircleCollisionComponent &circleCollisionComponent,
                                          const TransformComponent &transformComponent) {
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    auto &cameraPosition = cameraTransformComponent.position;
    auto &cameraSize = cameraComponent.size;
    auto &circlePosition = transformComponent.position;
    auto &circleSize = circleCollisionComponent.radius;
    auto sizeX = circleSize->getX() * transformComponent.scale->getX();
    auto sizeY = circleSize->getY() * transformComponent.scale->getY();

    if (circlePosition->getX() + sizeX / 2 < cameraPosition->getX() - cameraSize->getX() / 2 ||
        circlePosition->getX() - sizeX / 2 > cameraPosition->getX() + cameraSize->getX() / 2 ||
        circlePosition->getY() + sizeY / 2 < cameraPosition->getY() - cameraSize->getY() / 2 ||
        circlePosition->getY() - sizeY / 2 > cameraPosition->getY() + cameraSize->getY() / 2)
        return;

    SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);
    double angle = 0.0;
    double step = 0.01;  // Angle step for plotting points

    auto centerX = transformComponent.position->getX() - cameraTransformComponent.position->getX() +
                   cameraComponent.size->getX() / 2;
    auto centerY = transformComponent.position->getY() - cameraTransformComponent.position->getY() +
                   cameraComponent.size->getY() / 2;
    // Plot points along the ellipse boundary
    while (angle < 2 * M_PI) {
        int x = static_cast<int>(centerX + circleCollisionComponent.radius->getX() * cos(angle) *
                                           transformComponent.scale->getX() / 2);
        int y = static_cast<int>(centerY + circleCollisionComponent.radius->getY() * sin(angle) *
                                           transformComponent.scale->getY() / 2);

        SDL_RenderDrawPoint(renderer.get(), x, y);

        angle += step;
    }
#endif
}

void RenderWrapper::RenderBoxCollision(const CameraComponent &cameraComponent,
                                       const TransformComponent &cameraTransformComponent,
                                       const BoxCollisionComponent &boxCollisionComponent,
                                       const TransformComponent &transformComponent) {
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    auto &cameraPosition = cameraTransformComponent.position;
    auto &cameraSize = cameraComponent.size;
    auto &boxPosition = transformComponent.position;
    auto &size = boxCollisionComponent.size;
    auto sizeX = size->getX() * transformComponent.scale->getX();
    auto sizeY = size->getY() * transformComponent.scale->getY();

    if (boxPosition->getX() + sizeX / 2 < cameraPosition->getX() - cameraSize->getX() / 2 ||
        boxPosition->getX() - sizeX / 2 > cameraPosition->getX() + cameraSize->getX() / 2 ||
        boxPosition->getY() + sizeY / 2 < cameraPosition->getY() - cameraSize->getY() / 2 ||
        boxPosition->getY() - sizeY / 2 > cameraPosition->getY() + cameraSize->getY() / 2)
        return;

    SDL_Rect squareRect = {
            static_cast<int>(transformComponent.position->getX() - cameraTransformComponent.position->getX() +
                             cameraComponent.size->getX() / 2 - sizeX / 2),
            static_cast<int>(transformComponent.position->getY() - cameraTransformComponent.position->getY() +
                             cameraComponent.size->getY() / 2 - sizeY / 2),
            static_cast<int>(sizeX),
            static_cast<int>(sizeY)};

    // Render the button background (you can customize this part)
    SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer.get(), &squareRect);
#endif
}

void
RenderWrapper::RenderUiSprite(const SpriteComponent &spriteComponent, const TransformComponent &transformComponent) {
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

    //Create a rectangle were the sprite needs to be rendered on to
    SDL_Rect destRect = {static_cast<int>(transformComponent.position->getX()),
                         static_cast<int>(transformComponent.position->getY()),
                         static_cast<int>(spriteComponent.spriteSize->getX() * transformComponent.scale->getX()),
                         static_cast<int>(spriteComponent.spriteSize->getY() * transformComponent.scale->getY())};

    render(texture->second.get(), &srcRect, &destRect, transformComponent.rotation);
}

void RenderWrapper::RenderUiText(const TextComponent &textComponent, const TransformComponent &transformComponent) {
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
            std::string baseFontPath = ConfigSingleton::GetInstance().GetBaseAssetPath() + "Fonts/Arial.ttf";
            font = TTF_OpenFont(baseFontPath.c_str(), fontSize);
        }
        sizeMap[fontSize] = font;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font, textComponent.text.c_str(), sdlColor);

    if (!surface) {
        std::cerr << "TTF_RenderText_Solid error: " << TTF_GetError() << std::endl;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
    }

    SDL_Rect rect = {static_cast<int>(transformComponent.position->getX()),
                     static_cast<int>(transformComponent.position->getY()), surface->w, surface->h};

    render(texture,
           nullptr,
           &rect,
           transformComponent.rotation);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void RenderWrapper::RenderToMainTexture() {
    SDL_SetRenderTarget(renderer.get(), renderTexture.get());

    for (auto &cameraTexture: cameraTextures) {
        auto transformComp = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(cameraTexture.first);

        render(cameraTexture.second.second.get(),
               nullptr,
               &cameraTexture.second.first,
               transformComp.rotation);
    }
}

void
RenderWrapper::RenderSprite(const CameraComponent &cameraComponent, const TransformComponent &cameraTransformComponent,
                            const SpriteComponent &spriteComponent, const TransformComponent &transformComponent) {

    auto &cameraPosition = cameraTransformComponent.position;
    auto &cameraSize = cameraComponent.size;
    auto &spritePosition = transformComponent.position;
    auto &spriteSize = spriteComponent.spriteSize;
    auto sizeX = spriteSize->getX() * transformComponent.scale->getX();
    auto sizeY = spriteSize->getY() * transformComponent.scale->getY();

    if (spritePosition->getX() + sizeX / 2 < cameraPosition->getX() - cameraSize->getX() / 2 ||
        spritePosition->getX() - sizeX / 2 > cameraPosition->getX() + cameraSize->getX() / 2 ||
        spritePosition->getY() + sizeY / 2 < cameraPosition->getY() - cameraSize->getY() / 2 ||
        spritePosition->getY() - sizeY / 2 > cameraPosition->getY() + cameraSize->getY() / 2)
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


    auto width = spriteComponent.spriteSize->getX() * transformComponent.scale->getX();
    auto height = spriteComponent.spriteSize->getY() * transformComponent.scale->getY();
    //Create a rectangle were the sprite needs to be rendered on to
    SDL_Rect destRect = {
            static_cast<int>(transformComponent.position->getX() - cameraTransformComponent.position->getX() +
                             cameraComponent.size->getX() / 2 - width / 2),
            static_cast<int>(transformComponent.position->getY() - cameraTransformComponent.position->getY() +
                             cameraComponent.size->getY() / 2 - height / 2),
            static_cast<int>(width),
            static_cast<int>(height)};

    render(texture->second.get(),
           &srcRect,
           &destRect,
           transformComponent.rotation);
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
            std::string baseFontPath = ConfigSingleton::GetInstance().GetBaseAssetPath() + "Fonts/Arial.ttf";
            font = TTF_OpenFont(baseFontPath.c_str(), fontSize);
        }
        sizeMap[fontSize] = font;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font, textComponent.text.c_str(), sdlColor);

    if (!surface) {
        std::cerr << "TTF_RenderText_Solid error: " << TTF_GetError() << std::endl;
    }

    auto &cameraPosition = cameraTransformComponent.position;
    auto &cameraSize = cameraComponent.size;
    auto &textPosition = transformComponent.position;
    auto sizeX = surface->w;
    auto sizeY = surface->h;

    if (textPosition->getX() + sizeX / 2 < cameraPosition->getX() - cameraSize->getX() / 2 ||
        textPosition->getX() - sizeX / 2 > cameraPosition->getX() + cameraSize->getX() / 2 ||
        textPosition->getY() + sizeY / 2 < cameraPosition->getY() - cameraSize->getY() / 2 ||
        textPosition->getY() - sizeY / 2 > cameraPosition->getY() + cameraSize->getY() / 2)
        return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
    }

    SDL_Rect rect = {
            static_cast<int>(transformComponent.position->getX() - cameraTransformComponent.position->getX() +
                             cameraComponent.size->getX() / 2 - sizeX / 2),
            static_cast<int>(transformComponent.position->getY() - cameraTransformComponent.position->getY() +
                             cameraComponent.size->getY() / 2 - sizeY / 2),
            static_cast<int>(sizeX),
            static_cast<int>(sizeY)};

    render(texture, nullptr, &rect, transformComponent.rotation);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void RenderWrapper::RenderRectangle(const CameraComponent &cameraComponent,
                                    const TransformComponent &cameraTransformComponent,
                                    const RectangleComponent &rectangleComponent,
                                    const TransformComponent &transformComponent) {
    auto &cameraPosition = cameraTransformComponent.position;
    auto &cameraSize = cameraComponent.size;
    auto &rectanglePosition = transformComponent.position;
    auto &rectangleSize = rectangleComponent.size;
    auto sizeX = rectangleSize->getX() * transformComponent.scale->getX();
    auto sizeY = rectangleSize->getY() * transformComponent.scale->getY();

    if (rectanglePosition->getX() + sizeX / 2 < cameraPosition->getX() - cameraSize->getX() / 2 ||
        rectanglePosition->getX() - sizeX / 2 > cameraPosition->getX() + cameraSize->getX() / 2 ||
        rectanglePosition->getY() + sizeY / 2 < cameraPosition->getY() - cameraSize->getY() / 2 ||
        rectanglePosition->getY() - sizeY / 2 > cameraPosition->getY() + cameraSize->getY() / 2)
        return;

    SDL_Rect rect = {
            static_cast<int>(transformComponent.position->getX() - cameraTransformComponent.position->getX() +
                             cameraComponent.size->getX() / 2 - sizeX / 2),
            static_cast<int>(transformComponent.position->getY() - cameraTransformComponent.position->getY() +
                             cameraComponent.size->getY() / 2 - sizeY / 2),
            static_cast<int>(sizeX),
            static_cast<int>(sizeY)};

    SDL_SetRenderDrawColor(renderer.get(), rectangleComponent.fill->r, rectangleComponent.fill->g,
                           rectangleComponent.fill->b, rectangleComponent.fill->a);
    SDL_RenderFillRect(renderer.get(), &rect);

    if (rectangleComponent.borderWidth > 0) {
        SDL_SetRenderDrawColor(renderer.get(), rectangleComponent.borderColor->r, rectangleComponent.borderColor->g,
                               rectangleComponent.borderColor->b, rectangleComponent.borderColor->a);
        for (int i = 0; i < rectangleComponent.borderWidth; ++i) {
            SDL_Rect borderRect = {
                    static_cast<int>(transformComponent.position->getX() - cameraTransformComponent.position->getX() +
                                     cameraComponent.size->getX() / 2 - sizeX / 2 - i),
                    static_cast<int>(transformComponent.position->getY() - cameraTransformComponent.position->getY() +
                                     cameraComponent.size->getY() / 2 - sizeY / 2 - i),
                    static_cast<int>(sizeX + i * 2),
                    static_cast<int>(sizeY + i * 2)};
            SDL_RenderDrawRect(renderer.get(), &borderRect);
        }

    }
}

void RenderWrapper::RenderUiRectangle(const RectangleComponent &rectangleComponent,
                                      const TransformComponent &transformComponent) {
    SDL_Rect rect = {static_cast<int>(transformComponent.position->getX()),
                     static_cast<int>(transformComponent.position->getY()),
                     static_cast<int>(rectangleComponent.size->getX() * transformComponent.scale->getX()),
                     static_cast<int>(rectangleComponent.size->getY() * transformComponent.scale->getY())};

    SDL_SetRenderDrawColor(renderer.get(), rectangleComponent.fill->r, rectangleComponent.fill->g,
                           rectangleComponent.fill->b, rectangleComponent.fill->a);
    SDL_RenderFillRect(renderer.get(), &rect);

    if (rectangleComponent.borderWidth > 0) {
        SDL_SetRenderDrawColor(renderer.get(), rectangleComponent.borderColor->r, rectangleComponent.borderColor->g,
                               rectangleComponent.borderColor->b, rectangleComponent.borderColor->a);
        for (int i = 0; i < rectangleComponent.borderWidth; ++i) {
            SDL_Rect borderRect = {
                    static_cast<int>(transformComponent.position->getX() - i),
                    static_cast<int>(transformComponent.position->getY() - i),
                    static_cast<int>(rectangleComponent.size->getX() * transformComponent.scale->getX() + i * 2),
                    static_cast<int>(rectangleComponent.size->getY() * transformComponent.scale->getY() + i * 2)};
            SDL_RenderDrawRect(renderer.get(), &borderRect);
        }
    }
}

void RenderWrapper::render(SDL_Texture *texture, SDL_Rect *srcrect, SDL_Rect *dstrect, float rotation) const {
    int centerX = dstrect->w / 2;
    int centerY = dstrect->h / 2;
    SDL_Point rotationCenter = {centerX, centerY};

    SDL_RenderCopyEx(renderer.get(),
                     texture,
                     srcrect,
                     dstrect,
                     rotation,
                     &rotationCenter,
                     SDL_FLIP_NONE);
}

