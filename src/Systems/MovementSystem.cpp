//
// Created by jesse on 31/10/2023.
//

#include <Components/VelocityComponent.hpp>
#include "MovementSystem.hpp"
#include "../includes/ComponentStore.hpp"

MovementSystem::MovementSystem() {

}

MovementSystem::~MovementSystem() {

}

void MovementSystem::Update(float deltaTime) {
    auto entities = ComponentStore::GetInstance().getEntitiesWithComponent<VelocityComponent>();
    for (auto entity : entities) {
        auto velocityComponent = ComponentStore::GetInstance().getComponent<VelocityComponent>(entity);
        auto transformComponent = ComponentStore::GetInstance().getComponent<TransformComponent>(entity);
        transformComponent->position = std::make_unique<Vector2>(
                *transformComponent->position + velocityComponent->velocity * deltaTime
        );
        
        Logger::Info("Entity: " + std::to_string(entity) + " Position: " + std::to_string(transformComponent->position->getX()) + ", " + std::to_string(transformComponent->position->getY()));

    }
}

const std::string MovementSystem::GetName() const {
    return "MovementSystem";
}

void MovementSystem::CleanUp() {

}
