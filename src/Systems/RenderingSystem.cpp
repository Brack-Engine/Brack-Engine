//
// Created by jesse on 31/10/2023.
//

#include <Components/Archetypes/RenderArchetype.hpp>
#include "RenderingSystem.hpp"
#include "../includes/EntityManager.hpp"
#include "../includes/ComponentStore.hpp"

RenderingSystem::RenderingSystem() : sdl2Wrapper(new RenderWrapper()) {
}

RenderingSystem::~RenderingSystem() {

}

void RenderingSystem::update(milliseconds deltaTime) {
    SortRenderComponents();
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    auto boxCollisionComponentIds = ComponentStore::GetInstance().getEntitiesWithComponent<BoxCollisionComponent>();
    auto circleCollisionComponentIds = ComponentStore::GetInstance().getEntitiesWithComponent<CircleCollisionComponent>();
#endif
    auto cameras = ComponentStore::GetInstance().getEntitiesWithComponent<CameraComponent>();
    for (auto cameraId: cameras) {
        auto &cameraComponent = ComponentStore::GetInstance().tryGetComponent<CameraComponent>(cameraId);
        if (!cameraComponent.isActive)
            continue;
        auto &cameraTransformComponent = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(cameraId);
        sdl2Wrapper->RenderCamera(cameraComponent);
        for (auto component: components) {
            auto &transformComponent = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(
                    component->entityId);
            if (auto *tileMapComponent = dynamic_cast<const TileMapComponent *>(component))
                sdl2Wrapper->RenderTileMap(cameraComponent, cameraTransformComponent, *tileMapComponent,
                                           transformComponent);
            else if (auto *spriteComponent = dynamic_cast<const SpriteComponent *>(component))
                sdl2Wrapper->RenderSprite(cameraComponent, cameraTransformComponent, *spriteComponent,
                                          transformComponent);
            else if (auto *textComponent = dynamic_cast<const TextComponent *>(component))
                sdl2Wrapper->RenderText(cameraComponent, cameraTransformComponent, *textComponent, transformComponent);
            else if (auto *rectangleComponent = dynamic_cast<const RectangleComponent *>(component))
                sdl2Wrapper->RenderRectangle(cameraComponent, cameraTransformComponent, *rectangleComponent,
                                             transformComponent);
        }
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
        for (auto component: collisionComponents) {
            auto &transformComponent = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(
                    component->entityId);
            if (auto *boxCollisionComponent = dynamic_cast<const BoxCollisionComponent *>(component))
                sdl2Wrapper->RenderBoxCollision(cameraComponent, cameraTransformComponent, *boxCollisionComponent,
                                                transformComponent);
            else if (auto *circleCollisionComponent = dynamic_cast<const CircleCollisionComponent *>(component))
                sdl2Wrapper->RenderCircleCollision(cameraComponent, cameraTransformComponent,
                                                   *circleCollisionComponent, transformComponent);
        }
        auto graphComponentIds = ComponentStore::GetInstance().getEntitiesWithComponent<GraphComponent>();
        for (auto graphComponentId: graphComponentIds) {
            auto &graphComponent = ComponentStore::GetInstance().tryGetComponent<GraphComponent>(graphComponentId);
            auto &graphTransformComponent = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(
                    graphComponentId);
            sdl2Wrapper->RenderGraph(cameraComponent, cameraTransformComponent, graphComponent,
                                     graphTransformComponent);
        }
#endif
    }

    sdl2Wrapper->RenderToMainTexture();

    for (auto component: uiComponents) {
        auto &transformComponent = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(
                component->entityId);
        if (auto *tileMapComponent = dynamic_cast<const TileMapComponent *>(component))
            sdl2Wrapper->RenderUiTileMap(*tileMapComponent, transformComponent);
        else if (auto *spriteComponent = dynamic_cast<const SpriteComponent *>(component))
            sdl2Wrapper->RenderUiSprite(*spriteComponent, transformComponent);
        else if (auto *textComponent = dynamic_cast<const TextComponent *>(component))
            sdl2Wrapper->RenderUiText(*textComponent, transformComponent);
        else if (auto *rectangleComponent = dynamic_cast<const RectangleComponent *>(component))
            sdl2Wrapper->RenderUiRectangle(*rectangleComponent, transformComponent);
    }

#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    for (auto component: uiCollisionComponents) {
        auto &transformComponent = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(
                component->entityId);
        if (auto *boxCollisionComponent = dynamic_cast<const BoxCollisionComponent *>(component))
            sdl2Wrapper->RenderUiBoxCollision(*boxCollisionComponent, transformComponent);
        
    }
#endif

    sdl2Wrapper->RenderFrame();

}

void RenderingSystem::cleanUp() {
    sdl2Wrapper->Cleanup();
}

const std::string RenderingSystem::getName() const {
    return "RenderingSystem";
}

void RenderingSystem::SortRenderComponents() {
    components.clear();
    uiComponents.clear();
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    collisionComponents.clear();
    uiCollisionComponents.clear();
#endif

    auto tileMapComponentIds = ComponentStore::GetInstance().getEntitiesWithComponent<TileMapComponent>();
    for (auto entityId: tileMapComponentIds) {
        auto &tileMapComponent = ComponentStore::GetInstance().tryGetComponent<TileMapComponent>(entityId);
        if (!tileMapComponent.isActive)
            continue;
        components.insert(&tileMapComponent);
    }

    auto spriteComponentIds = ComponentStore::GetInstance().getEntitiesWithComponent<SpriteComponent>();
    for (auto entityId: spriteComponentIds) {
        auto &spriteComponent = ComponentStore::GetInstance().tryGetComponent<SpriteComponent>(entityId);
        if (!spriteComponent.isActive)
            continue;

        if (spriteComponent.sortingLayer == 0) {
            uiComponents.insert(&spriteComponent);
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
            try {
                auto &boxCollisionComponent = ComponentStore::GetInstance().tryGetComponent<BoxCollisionComponent>(
                        entityId);
                uiCollisionComponents.insert(&boxCollisionComponent);
            } catch (std::exception &e) {}
            try {
                auto &circleCollisionComponent = ComponentStore::GetInstance().tryGetComponent<CircleCollisionComponent>(
                        entityId);
                uiCollisionComponents.insert(&circleCollisionComponent);
            } catch (std::exception &e) {}
#endif
        }// UI layer
        else {
            components.insert(&spriteComponent);
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
            try {
                auto &boxCollisionComponent = ComponentStore::GetInstance().tryGetComponent<BoxCollisionComponent>(
                        entityId);
                collisionComponents.insert(&boxCollisionComponent);
            } catch (std::exception &e) {}
            try {
                auto &circleCollisionComponent = ComponentStore::GetInstance().tryGetComponent<CircleCollisionComponent>(
                        entityId);
                collisionComponents.insert(&circleCollisionComponent);
            } catch (std::exception &e) {}
#endif
        }
    }
    auto textComponentIds = ComponentStore::GetInstance().getEntitiesWithComponent<TextComponent>();
    for (auto entityId: textComponentIds) {
        auto &textComponent = ComponentStore::GetInstance().tryGetComponent<TextComponent>(entityId);
        if (!textComponent.isActive)
            continue;
        if (textComponent.sortingLayer == 0) {
            uiComponents.insert(&textComponent);
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
            try {
                auto &boxCollisionComponent = ComponentStore::GetInstance().tryGetComponent<BoxCollisionComponent>(
                        entityId);
                uiCollisionComponents.insert(&boxCollisionComponent);
            } catch (std::exception &e) {}
            try {
                auto &circleCollisionComponent = ComponentStore::GetInstance().tryGetComponent<CircleCollisionComponent>(
                        entityId);
                uiCollisionComponents.insert(&circleCollisionComponent);
            } catch (std::exception &e) {}
#endif
        } else {
            components.insert(&textComponent);
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
            try {
                auto &boxCollisionComponent = ComponentStore::GetInstance().tryGetComponent<BoxCollisionComponent>(
                        entityId);
                collisionComponents.insert(&boxCollisionComponent);
            } catch (std::exception &e) {}
            try {
                auto &circleCollisionComponent = ComponentStore::GetInstance().tryGetComponent<CircleCollisionComponent>(
                        entityId);
                collisionComponents.insert(&circleCollisionComponent);
            } catch (std::exception &e) {}
#endif
        }
    }
    auto RectangleComponentIds = ComponentStore::GetInstance().getEntitiesWithComponent<RectangleComponent>();
    for (auto entityId: RectangleComponentIds) {
        auto &rectangleComponent = ComponentStore::GetInstance().tryGetComponent<RectangleComponent>(entityId);
        if (!rectangleComponent.isActive)
            continue;
        if (rectangleComponent.sortingLayer == 0) {
            uiComponents.insert(&rectangleComponent);
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
            try {
                auto &boxCollisionComponent = ComponentStore::GetInstance().tryGetComponent<BoxCollisionComponent>(
                        entityId);
                uiCollisionComponents.insert(&boxCollisionComponent);
            } catch (std::exception &e) {}
            try {
                auto &circleCollisionComponent = ComponentStore::GetInstance().tryGetComponent<CircleCollisionComponent>(
                        entityId);
                uiCollisionComponents.insert(&circleCollisionComponent);
            } catch (std::exception &e) {}
#endif
        } else {
            components.insert(&rectangleComponent);
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
            try {
                auto &boxCollisionComponent = ComponentStore::GetInstance().tryGetComponent<BoxCollisionComponent>(
                        entityId);
                collisionComponents.insert(&boxCollisionComponent);
            } catch (std::exception &e) {}
            try {
                auto &circleCollisionComponent = ComponentStore::GetInstance().tryGetComponent<CircleCollisionComponent>(
                        entityId);
                collisionComponents.insert(&circleCollisionComponent);
            } catch (std::exception &e) {}
#endif
        }
    }
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
    auto boxCollisionComponentIds = ComponentStore::GetInstance().getEntitiesWithComponent<BoxCollisionComponent>();
    for (auto entityId: boxCollisionComponentIds) {
        auto &boxCollisionComponent = ComponentStore::GetInstance().tryGetComponent<BoxCollisionComponent>(entityId);
        if (!boxCollisionComponent.isActive)
            continue;
        collisionComponents.insert(&boxCollisionComponent);
    }
#endif
}

void RenderingSystem::clearCache() {
    sdl2Wrapper->cleanCache();
    components.clear();
}

RenderingSystem::RenderingSystem(const RenderingSystem &other) {
    sdl2Wrapper = std::make_unique<RenderWrapper>();
}

