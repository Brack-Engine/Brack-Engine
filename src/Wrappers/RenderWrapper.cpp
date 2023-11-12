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
    SDL_Window *window = SDL_CreateWindow(ConfigSingleton::GetInstance().GetWindowTitle().c_str(),
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, ConfigSingleton::GetInstance().GetWindowSize().getX(),
                                          ConfigSingleton::GetInstance().GetWindowSize().getY(), SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        Cleanup();
        return false;
    }

    // Perform additional initialization as needed (e.g., renderer setup, resource loading).
    renderer = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>(
            SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED),
            [](SDL_Renderer *r) { SDL_DestroyRenderer(r); }
    );

    if (renderer == nullptr) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        Logger::GetInstance().Shutdown();
        SDL_DestroyWindow(window);
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

void RenderWrapper::RenderCamera(CameraComponent *camera) {
    auto &backgroundColor = camera->backgroundColor;
    SDL_SetRenderDrawColor(renderer.get(), backgroundColor.r, backgroundColor.g, backgroundColor.b,
                           backgroundColor.a); // RGBA format

    // Clear the screen with the background color.
    SDL_RenderClear(renderer.get());
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
    SDL_Rect destRect = {static_cast<int>(sprite.position->getX()), static_cast<int>(sprite.position->getY()), static_cast<int>(sprite.spriteSize->getX() * sprite.scale->getX()), static_cast<int>(sprite.spriteSize->getY() * sprite.scale->getY())};

    SDL_RenderCopy(renderer.get(), texture->second.get(), &srcRect, &destRect);
}

void RenderWrapper::RenderText(TextComponent* textComponent, TransformComponent* transformComponent) {
    SDL_Color sdlColor = {
            static_cast<Uint8>(textComponent->color->r),
            static_cast<Uint8>(textComponent->color->g),
            static_cast<Uint8>(textComponent->color->b),
            static_cast<Uint8>(textComponent->color->a)
    };

    TTF_Font* font = nullptr;
    const std::string& fontPath = textComponent->fontPath;
    int fontSize = textComponent->fontSize;

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

    SDL_Surface* surface = TTF_RenderText_Solid(font, textComponent->text.c_str(), sdlColor);

    if (!surface) {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }

    SDL_Rect rect = { static_cast<int>(transformComponent->position->getX()), static_cast<int>(transformComponent->position->getY()), surface->w, surface->h };
    SDL_RenderCopy(renderer.get(), texture, nullptr, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void RenderWrapper::RenderButton(TextComponent &button) {

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
