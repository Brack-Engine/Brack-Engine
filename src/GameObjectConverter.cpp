//
// Created by jesse on 04/11/2023.
//

#include <Components/ChildComponent.hpp>
#include <Components/ParentComponent.hpp>
#include <Components/BehaviourScript.hpp>
#include "GameObjectConverter.hpp"
#include "includes/EntityManager.hpp"
#include "Components/ComponentVisitor.hpp"


void GameObjectConverter::addGameObject(GameObject *gameObject) {
    ComponentVisitor componentVisitor;
    auto entityId = gameObject->getEntityId();
    if (gameObject->getEntityId() == 0)
        entityId = EntityManager::getInstance().createEntity();

    for (const auto &child: gameObject->getChildren()) {
        try {
            auto &parentComponent = child->tryGetComponent<ParentComponent>();
            parentComponent.parentId = entityId;
        } catch (std::runtime_error &e) {}
    }

    auto parent = gameObject->getParent();
    if (parent.has_value()) {
        auto &parentComponent = parent.value().tryGetComponent<ChildComponent>();
        parentComponent.children.emplace_back(entityId);
    }

    gameObject->setEntityId(entityId);
    std::vector<std::unique_ptr<IComponent>> components = std::move(gameObject->getAllComponents());
    for (auto &component: components) {
        component->accept(componentVisitor);
        ComponentStore::GetInstance().addComponent(entityId, std::move(component));
    }

    auto scripts = ComponentStore::GetInstance().getAllComponentsOfType<BehaviourScript>();
    for (auto &script: scripts) {
        script->onStart();
    }

}

std::optional<GameObject> GameObjectConverter::getGameObjectByName(const std::string &name) {
    auto entityId = EntityManager::getInstance().getEntityByName(name);
    if (entityId == 0)
        return std::nullopt;

    return GameObject(entityId);
}

std::vector<GameObject> GameObjectConverter::getGameObjectsByName(const std::string &name) {
    auto gameObjects = std::vector<GameObject>();
    auto entityIds = EntityManager::getInstance().getEntitiesByName(name);
    for (auto entityId: entityIds) {
        gameObjects.emplace_back(entityId);
    }
    return gameObjects;
}

std::optional<GameObject> GameObjectConverter::getGameObjectByTag(const std::string &tag) {
    auto entityId = EntityManager::getInstance().getEntityByTag(tag);
    if (entityId == 0)
        return std::nullopt;

    return GameObject(entityId);
}

std::vector<GameObject> GameObjectConverter::getGameObjectsByTag(const std::string &tag) {
    auto gameObjects = std::vector<GameObject>();
    auto entityIds = EntityManager::getInstance().getEntitiesByTag(tag);
    for (auto entityId: entityIds) {
        gameObjects.emplace_back(entityId);
    }
    return gameObjects;
}

std::vector<GameObject> GameObjectConverter::getChildren(entity entityID) {
    auto children = std::vector<GameObject>();
    try {
        auto &childComponent = ComponentStore::GetInstance().tryGetComponent<ChildComponent>(entityID);
        for (auto childId: childComponent.children) {
            children.emplace_back(childId);
        }
        return children;
    } catch (std::runtime_error &e) {
        return children;
    }
}

std::optional<GameObject> GameObjectConverter::getParent(entity entityID) {
    try {
        auto &parentComponent = ComponentStore::GetInstance().tryGetComponent<ParentComponent>(entityID);
        return GameObject(parentComponent.parentId);
    } catch (std::runtime_error &e) {
        return std::nullopt;
    }
}
