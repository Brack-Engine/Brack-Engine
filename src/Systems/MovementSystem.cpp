//
// Created by jesse on 31/10/2023.
//

#include <Components/VelocityComponent.hpp>
#include <Components/ChildComponent.hpp>
#include "MovementSystem.hpp"
#include "../includes/ComponentStore.hpp"

MovementSystem::MovementSystem() {

}

MovementSystem::~MovementSystem() {

}

void MovementSystem::update(milliseconds deltaTime) {
    auto entities = ComponentStore::GetInstance().getEntitiesWithComponent<VelocityComponent>();
    for (auto entity: entities) {
        auto velocityComponent = ComponentStore::GetInstance().tryGetComponent<VelocityComponent>(entity);
        if (velocityComponent.velocity == Vector2(0, 0)) {
            continue;
        }
        auto &transformComponent = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(entity);
        transformComponent.position = std::make_unique<Vector2>(
                *transformComponent.position + velocityComponent.velocity * deltaTime
        );
    }
}

const std::string MovementSystem::getName() const {
    return "MovementSystem";
}

void MovementSystem::cleanUp() {

}

