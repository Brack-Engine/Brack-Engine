#include <SDL_image.h>
#include "RenderWrapper.hpp"
#include "../includes/SystemManager.hpp"
#include "../ConfigSingleton.hpp"
#include "../Logger.hpp"

RenderWrapper::RenderWrapper() : renderer(nullptr, nullptr) {
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
        Logger::GetInstance().Shutdown();
        SDL_DestroyWindow(window.get());
        Cleanup();
        return false;
    }

    return true;
}

void RenderWrapper::Cleanup() {
    // Perform cleanup as necessary
    Logger::GetInstance().Shutdown();
    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
}

void RenderWrapper::RenderCamera(CameraComponent& camera) {
    auto &backgroundColor = camera.backgroundColor;
    SDL_SetRenderDrawColor(renderer.get(), backgroundColor->r, backgroundColor->g, backgroundColor->b,
                           backgroundColor->a); // RGBA format

    // Clear the screen with the background color.
    SDL_RenderClear(renderer.get());

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window.get(), &windowWidth, &windowHeight);

    if(windowWidth != camera.size->getX() || windowHeight != camera.size->getY()) {
        int centerX = SDL_WINDOWPOS_CENTERED;
        int centerY = SDL_WINDOWPOS_CENTERED;
        SDL_SetWindowSize(window.get(), camera.size->getX(), camera.size->getY());
        SDL_SetWindowPosition(window.get(), centerX, centerY);
    }
}

void RenderWrapper::RenderRectangle(RectangleComponent& rectangleComponent, TransformComponent& transformComponent) {
    SDL_Rect rectFill = {
            static_cast<int>(transformComponent.position->getX()),
            static_cast<int>(transformComponent.position->getY()),
            static_cast<int>(rectangleComponent.size->getX()),
            static_cast<int>(rectangleComponent.size->getY()) };

    // Render background
    SDL_SetRenderDrawColor(
            renderer.get(),
            rectangleComponent.fill->r,
            rectangleComponent.fill->g,
            rectangleComponent.fill->b,
            rectangleComponent.fill->a
            );
    SDL_RenderFillRect(renderer.get(), &rectFill);

    if(rectangleComponent.borderWidth > 0){
        // Render the border borderWidth times
        SDL_SetRenderDrawColor(
                renderer.get(),
               rectangleComponent.borderColor->r,
               rectangleComponent.borderColor->g,
               rectangleComponent.borderColor->b,
               rectangleComponent.borderColor->a
               );
        for (int i = 0; i < rectangleComponent.borderWidth; ++i) {
            SDL_Rect rectBorder = {
                    static_cast<int>(transformComponent.position->getX()) + i,
                    static_cast<int>(transformComponent.position->getY()) + i,
                    static_cast<int>(rectangleComponent.size->getX()) - (i*2),
                    static_cast<int>(rectangleComponent.size->getY()) - (i*2)
            };
            SDL_RenderDrawRect(renderer.get(), &rectBorder);
        }
    }
}

void RenderWrapper::RenderSprite(SpriteComponent &sprite) {
    //Check if the texture is already created. If not add it to the created textures
    if(textures.find(sprite.spritePath) == textures.end())
        textures.insert(std::make_pair(sprite.spritePath, getTexture(sprite.spritePath)));
    auto texture = textures.find(sprite.spritePath);

    //Fill in a rectangle for the current sprite IN
    SDL_Rect srcRect;
    int spriteWidth = sprite.spriteSize->getX();
    int spriteHeight = sprite.spriteSize->getY();
    srcRect.x = (sprite.tileOffset->getX() * spriteWidth) + (sprite.margin * sprite.tileOffset->getX());
    srcRect.y = (sprite.tileOffset->getY() * spriteHeight) + (sprite.margin * sprite.tileOffset->getY());
    srcRect.w = spriteWidth;
    srcRect.h = spriteHeight;

    //Create a rectangle were the sprite needs to be rendered on to
    SDL_Rect destRect = {
            static_cast<int>(sprite.position->getX()),
            static_cast<int>(sprite.position->getY()),
            static_cast<int>(sprite.spriteSize->getX() * sprite.scale->getX()),
            static_cast<int>(sprite.spriteSize->getY() * sprite.scale->getY())
    };

    SDL_RenderCopy(renderer.get(), texture->second.get(), &srcRect, &destRect);
}

void RenderWrapper::RenderText(TextComponent& textComponent, TransformComponent& transformComponent) {
    SDL_Color sdlColor = {
            textComponent.color->r,
            textComponent.color->g,
            textComponent.color->b,
            textComponent.color->a
    };

    TTF_Font* font = nullptr;
    const std::string& fontPath = textComponent.fontPath;
    int fontSize = textComponent.fontSize;

    auto& sizeMap = fontCache[fontPath];
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

    SDL_Surface* surface = TTF_RenderText_Solid(font, textComponent.text.c_str(), sdlColor);

    if (!surface) {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }

    SDL_Rect rect = { static_cast<int>(transformComponent.position->getX()), static_cast<int>(transformComponent.position->getY()), surface->w, surface->h };
    SDL_RenderCopy(renderer.get(), texture, nullptr, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void RenderWrapper::RenderBoxCollisionComponents(BoxCollisionComponent& boxCollisionComponent,
                                                 TransformComponent& transformComponent) {
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    SDL_Rect buttonRect = {
            static_cast<int>(transformComponent.position->getX()),
            static_cast<int>(transformComponent.position->getY()),
            static_cast<int>(boxCollisionComponent.size->getX()),
            static_cast<int>(boxCollisionComponent.size->getY()) };

    // Render the button background (you can customize this part)
    SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer.get(), &buttonRect);
#endif
}

void RenderWrapper::RenderCircleCollisionComponents(CircleCollisionComponent& circleCollisionComponent, TransformComponent& transformComponent){
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    SDL_SetRenderDrawColor(renderer.get(), 255, 0, 0, 255);
    double angle = 0.0;
    double step = 0.005;  // Angle step for plotting points

    auto centerX = transformComponent.position->getX() + circleCollisionComponent.radius->getX();
    auto centerY = transformComponent.position->getY() + circleCollisionComponent.radius->getY();
    // Plot points along the ellipse boundary
    while (angle < 2 * M_PI) {
        int x = static_cast<int>(centerX + circleCollisionComponent.radius->getX() * cos(angle));
        int y = static_cast<int>(centerY + circleCollisionComponent.radius->getY() * sin(angle));

        SDL_RenderDrawPoint(renderer.get(), x, y);

        angle += step;
    }
#endif
}

void RenderWrapper::RenderFrame() {
    SDL_RenderPresent(renderer.get());
}

std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> RenderWrapper::getTexture(std::string filePath) {
    // Get the file extension
    size_t dotPos = filePath.find_last_of('.');
    if (dotPos == std::string::npos) {
        std::cerr << "Error: Invalid file path (no file extension)" << std::endl;
        return std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(nullptr, &SDL_DestroyTexture);
    }

    std::string extension = filePath.substr(dotPos + 1);

    if (extension == "bmp") {
        SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());
        if (surface) {
            std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> bmpTexture(SDL_CreateTextureFromSurface(renderer.get(), surface), &SDL_DestroyTexture);
            SDL_FreeSurface(surface);
            return std::move(bmpTexture);
        } else {
            std::cerr << "Error: Failed to load BMP file: " << SDL_GetError() << std::endl;
        }
    } else if (extension == "png") {
        SDL_Texture* pngTexture = IMG_LoadTexture(renderer.get(), filePath.c_str());
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

void RenderWrapper::ResizeWindow(Vector2 size) {
    SDL_SetWindowSize(window.get(),(int)size.getX(),(int)size.getY());
    ConfigSingleton::GetInstance().SetWindowSize(size);
}