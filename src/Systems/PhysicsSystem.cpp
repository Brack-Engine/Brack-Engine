//
// Created by jesse on 31/10/2023.
//

#include <Components/CircleCollisionComponent.hpp>
#include <Components/BoxCollisionComponent.hpp>
#include "PhysicsSystem.hpp"
#include "../includes/ComponentStore.hpp"

PhysicsSystem::PhysicsSystem() {
}

PhysicsSystem::~PhysicsSystem() {

}

void PhysicsSystem::update(milliseconds deltaTime) {
    handleCircles();
    handleBoxes();

    PhysicsWrapper::getInstance().update(deltaTime);

}

void PhysicsSystem::cleanUp() {
}

const std::string PhysicsSystem::getName() const {
    return "PhysicsSystem";
}


void PhysicsSystem::handleCircles() {
    auto &compStore = ComponentStore::GetInstance();

    auto circleCollisionComponents = compStore.getAllComponentsOfType<CircleCollisionComponent>();
    if (circleCollisionComponents.empty()) return;
    PhysicsWrapper::getInstance().addCircles(circleCollisionComponents);
}

void PhysicsSystem::handleBoxes() {
    auto &compStore = ComponentStore::GetInstance();

    auto boxCollisionComponents = compStore.getAllComponentsOfType<BoxCollisionComponent>();

    if (boxCollisionComponents.empty()) return;
    PhysicsWrapper::getInstance().addBoxes(boxCollisionComponents);
}

void PhysicsSystem::clearCache() {
    PhysicsWrapper::getInstance().cleanCache();
}






