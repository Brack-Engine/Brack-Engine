//
// Created by jesse on 30/10/2023.
//

#ifndef BRACK_ENGINE_RENDERWRAPPER_HPP
#define BRACK_ENGINE_RENDERWRAPPER_HPP

#include <iostream>
#include "SDL.h"
#include <SDL_ttf.h>
#include "Components/CameraComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/TextComponent.hpp"
#include <map>
#include <memory>
#include <Components/BoxCollisionComponent.hpp>
#include <Components/CircleCollisionComponent.hpp>
#include <Components/RectangleComponent.hpp>
#include <unordered_map>
#include <Components/TransformComponent.hpp>
#include <Components/TileMapComponent.hpp>
#include <Components/GraphComponent.hpp>

struct SDLWindowDeleter {
    void operator()(SDL_Window *window) const {
        SDL_DestroyWindow(window);
    }
};

class RenderWrapper {
public:
    RenderWrapper();

    ~RenderWrapper();

    void RenderCamera(const CameraComponent &cameraComponent);

    void RenderTileMap(const CameraComponent &cameraComponent, const TransformComponent &cameraTransformComponent,
                       const TileMapComponent &tileMapComponent, const TransformComponent &transformComponent);

    void RenderSprite(const CameraComponent &cameraComponent, const TransformComponent &cameraTransformComponent,
                      const SpriteComponent &spriteComponent, const TransformComponent &transformComponent);

    void RenderText(const CameraComponent &cameraComponent, const TransformComponent &cameraTransformComponent,
                    const TextComponent &textComponent, const TransformComponent &transformComponent);

    void RenderRectangle(const CameraComponent &cameraComponent, const TransformComponent &cameraTransformComponent,
                         const RectangleComponent &rectangleComponent, const TransformComponent &transformComponent);

    void RenderBoxCollision(const CameraComponent &cameraComponent, const TransformComponent &cameraTransformComponent,
                            const BoxCollisionComponent &boxCollisionComponent,
                            const TransformComponent &transformComponent);

    void
    RenderCircleCollision(const CameraComponent &cameraComponent, const TransformComponent &cameraTransformComponent,
                          const CircleCollisionComponent &circleCollisionComponent,
                          const TransformComponent &transformComponent);

    void RenderUiTileMap(const TileMapComponent &tileMapComponent, const TransformComponent &transformComponent);

    void RenderUiBoxCollision(const BoxCollisionComponent &boxCollisionComponent,
                              const TransformComponent &transformComponent);

    void RenderUiSprite(const SpriteComponent &spriteComponent, const TransformComponent &transformComponent);

    void RenderUiText(const TextComponent &textComponent, const TransformComponent &transformComponent);

    void RenderUiRectangle(const RectangleComponent &rectangleComponent, const TransformComponent &transformComponent);

    void RenderGraph(const CameraComponent &cameraComponent, const TransformComponent &cameraTransformComponent,
                     const GraphComponent &graphComponent,
                     const TransformComponent &graphTransformComponent);

    void RenderToMainTexture();

    void RenderFrame();

    static void Cleanup();

    void cleanCache();

    void ResizeWindow(Vector2 size);

private:
    bool Initialize();

    void render(SDL_Texture *texture, SDL_Rect *srcrect, SDL_Rect *dstrect, float rotation, const bool flipX,
                const bool flipY) const;

    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> GetSpriteTexture(std::string filePath);

    std::pair<SDL_Rect, std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)>> &
    GetCameraTexturePair(const CameraComponent &cameraComponent);

    std::map<entity, std::pair<SDL_Rect, std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>>> cameraTextures;
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> renderTexture;
    std::unordered_map<std::string, std::map<int, TTF_Font *>> fontCache;
    std::map<std::string, std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>> textures;
    std::unique_ptr<SDL_Window, SDLWindowDeleter> window;
    std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> renderer;
};


#endif //BRACK_ENGINE_RENDERWRAPPER_HPP
