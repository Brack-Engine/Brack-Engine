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
}

void RenderWrapper::RenderCamera(CameraComponent *camera) {
    auto &backgroundColor = camera->backgroundColor;
    SDL_SetRenderDrawColor(renderer.get(), backgroundColor.r, backgroundColor.g, backgroundColor.b,
                           backgroundColor.a); // RGBA format

    // Clear the screen with the background color.
    SDL_RenderClear(renderer.get());
}

void RenderWrapper::RenderSprite(SpriteComponent &sprite) {

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
