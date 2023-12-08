//
// Created by jesse on 04/11/2023.
//

#include <Components/ChildComponent.hpp>
#include <Components/ParentComponent.hpp>
#include <Components/BehaviourScript.hpp>
#include "GameObjectConverter.hpp"
#include "includes/EntityManager.hpp"


void GameObjectConverter::addGameObject(std::unique_ptr<GameObject> &gameObject) {
    auto entityId = gameObject->getEntityId();
    if (gameObject->getEntityId() == 0)
        entityId = EntityManager::getInstance().createEntity();

    auto children = std::move(gameObject->getChildren());

    for (auto &child: children) {
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
        if (auto *objectInfoComponent = dynamic_cast<ObjectInfoComponent *>(component.get())) {
            if (!objectInfoComponent->name.empty()) {
                EntityManager::getInstance().addEntityWithName(entityId, objectInfoComponent->name);
            }
            if (!objectInfoComponent->tag.empty()) {
                EntityManager::getInstance().addEntityWithTag(entityId, objectInfoComponent->tag);
            }
            EntityManager::getInstance().setEntityActive(entityId, objectInfoComponent->isActive);
        }
        ComponentStore::GetInstance().addComponent(entityId, std::move(component));
    }

    for (auto &child: children) {
        child->setEntityId(EntityManager::getInstance().createEntity());
        addGameObject(child);
    }

}

std::optional<GameObject> GameObjectConverter::getGameObjectByName(const std::string &name) {
    auto entityId = EntityManager::getInstance().getEntityByName(name);
    if (entityId == 0)
        return std::nullopt;

    return GameObject(entityId);
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
        auto gameObject = GameObject(entityId);
        gameObjects.emplace_back(gameObject);
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

void GameObjectConverter::removeGameObject(std::unique_ptr<GameObject> &gameObject) {
    auto children = gameObject->getChildren();
    for (auto &child: children) {
        removeGameObject(child);
    }

    auto parent = gameObject->getParent();
    if (parent.has_value()) {
        auto &parentComponent = parent.value().tryGetComponent<ChildComponent>();
        parentComponent.children.erase(
                std::remove(parentComponent.children.begin(), parentComponent.children.end(),
                            gameObject->getEntityId()),
                parentComponent.children.end());
    }

    ComponentStore::GetInstance().removeComponentsOfEntity(gameObject->getEntityId());
    EntityManager::getInstance().destroyEntity(gameObject->getEntityId());
}

