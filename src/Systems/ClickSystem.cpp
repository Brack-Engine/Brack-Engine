//
// Created by SKIKK on 10/11/2023.
//

#include <EngineManagers/InputManager.hpp>
#include <Helpers/MouseMap.hpp>
#include <Components/BoxCollisionComponent.hpp>
#include <Components/CircleCollisionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include "ClickSystem.hpp"
#include "../includes/ComponentStore.hpp"

ClickSystem::ClickSystem() {}

ClickSystem::~ClickSystem() {}

void ClickSystem::update(milliseconds deltaTime) {
    if (!InputManager::GetInstance().IsMouseReleased(LEFT_MOUSE)) return;

    auto &componentStore = ComponentStore::GetInstance();
    auto &mousePosition = InputManager::GetInstance().getScreenMousePosition();
    auto clickableComponentIds = componentStore.getEntitiesWithComponent<ClickableComponent>();
    for (int entityId: clickableComponentIds) {
        auto clickableComponent = componentStore.tryGetComponent<ClickableComponent>(entityId);
        if (clickableComponent.disabled) continue;
        CheckBoxCollision(clickableComponent, mousePosition);
        CheckCircleCollision(clickableComponent, mousePosition);
    }
}

void ClickSystem::CheckBoxCollision(const ClickableComponent &clickableComponent,
                                    const Vector2 &mousePosition) {
    try {
        auto boxColliderComponent = ComponentStore::GetInstance().tryGetComponent<BoxCollisionComponent>(
                clickableComponent.entityId);
        auto transformComponent = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(
                clickableComponent.entityId);
        if (
                mousePosition.getX() >= transformComponent.position->getX() &&
                mousePosition.getX() <= transformComponent.position->getX() + boxColliderComponent.size->getX() &&
                mousePosition.getY() >= transformComponent.position->getY() &&
                mousePosition.getY() <= transformComponent.position->getY() + boxColliderComponent.size->getY()
                ) {
            clickableComponent.OnClick();
        }
    } catch (const std::exception &e) {

    }
}

void ClickSystem::CheckCircleCollision(const ClickableComponent &clickableComponent, const Vector2 &mousePosition) {
//    try {
//        auto circleCollisionComponent = ComponentStore::GetInstance().tryGetComponent<CircleCollisionComponent>(
//                clickableComponent.entityId);
//        auto transformComponent = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(
//                clickableComponent.entityId);
//        auto x = mousePosition.getX();
//        auto y = mousePosition.getY();
//        auto a = circleCollisionComponent.radius->getX();
//        auto b = circleCollisionComponent.radius->getY();
//        auto h = transformComponent.position->getX() + circleCollisionComponent.radius->getX();
//        auto k = transformComponent.position->getY() + circleCollisionComponent.radius->getY();
//        // Calculate the left-hand side of the ellipse equation
//        double lhs = ((x - h) * (x - h)) / (a * a) + ((y - k) * (y - k)) / (b * b);
//
//        // Check if the point is inside the ellipse
//        if (lhs <= 1.0) {
//            clickableComponent.OnClick();
//        }
//    } catch (const std::exception &e) {
//
//    }
}

const std::string ClickSystem::getName() const {
    return "ClickSystem";
}

void ClickSystem::cleanUp() {

}

void ClickSystem::clearCache() {

}
