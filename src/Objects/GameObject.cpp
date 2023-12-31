//
// Created by jesse on 31/10/2023.
//

#include <Components/TransformComponent.hpp>
#include <Components/ObjectInfoComponent.hpp>
#include <Components/ParentComponent.hpp>
#include <Components/ChildComponent.hpp>
#include "Objects/GameObject.hpp"
#include "../GameObjectConverter.hpp"
#include <algorithm>
#include <utility>
#include <optional>

GameObject::GameObject() {
    addComponent(std::make_unique<TransformComponent>());
    addComponent(std::make_unique<ObjectInfoComponent>());
}

GameObject::GameObject(entity id) {
    entityID = id;
}

std::optional<GameObject *> GameObject::getChildGameObjectByName(const std::string name) {
    for (auto &gameObject: children) {
        if (gameObject->getName() == name)
            return gameObject.get();
    }
    auto childComponent = ComponentStore::GetInstance().tryGetComponent<ChildComponent>(entityID);
    auto it = std::find(childComponent.children.begin(), childComponent.children.end(),
                        EntityManager::getInstance().getEntityByName(name));
    if (it != childComponent.children.end()) {
        return new GameObject(*it);
    }
    return std::nullopt;
}

std::vector<std::unique_ptr<GameObject>> &&GameObject::getChildren() {
    if (entityID == 0) {
        return std::move(children);
    }
    try {
        auto &childComponent = tryGetComponent<ChildComponent>();
        for (auto &childId: childComponent.children) {
            auto child = std::make_unique<GameObject>(childId);
            children.push_back(std::move(child));
        }
        return std::move(children);
    } catch (std::runtime_error &e) {
        return std::move(children);
    }
}

std::optional<GameObject> GameObject::getParent() {
    if (parent == nullptr) {
        try {
            auto &parentComponent = tryGetComponent<ParentComponent>();
            return GameObject(parentComponent.parentId);
        } catch (std::runtime_error &e) {
            return std::nullopt;
        }
    }
    return *parent;


}

std::string GameObject::getName() const {
    return tryGetComponent<ObjectInfoComponent>().name;
}


void GameObject::setName(std::string name) const {
    tryGetComponent<ObjectInfoComponent>().name = std::move(name);
}

std::string GameObject::getTag() const {
    return tryGetComponent<ObjectInfoComponent>().tag;
}

void GameObject::setTag(std::string name) const {
    tryGetComponent<ObjectInfoComponent>().tag = std::move(name);
}

bool GameObject::isActive() const {
    return EntityManager::getInstance().isEntityActive(entityID) && tryGetComponent<ObjectInfoComponent>().isActive;
}

void GameObject::setActive(bool active) {
    tryGetComponent<ObjectInfoComponent>().isActive = active;
    EntityManager::getInstance().setEntityActive(entityID, active);

    if (entityID == 0) {
        for (auto &child: children) {
            EntityManager::getInstance().setEntityActive(child->getEntityId(), active);
        }
    } else {
        childrenSetActive(entityID, active);
    }
}

void GameObject::childrenSetActive(entity entityId, bool active) {
    try {
        EntityManager::getInstance().setEntityActive(entityId, active);
        auto &childComponent = ComponentStore::GetInstance().tryGetComponent<ChildComponent>(entityId);
        for (auto &childId: childComponent.children) {
            childrenSetActive(childId, active);
        }
    } catch (std::runtime_error &e) {
        return;
    }
}


int GameObject::getLayer() const {
    return tryGetComponent<ObjectInfoComponent>().layer;
}

void GameObject::setLayer(int layer) const {
    tryGetComponent<ObjectInfoComponent>().layer = layer;
}

entity GameObject::getEntityId() const {
    return entityID;
}

void GameObject::setEntityId(entity id) {
    entityID = id;
    for (auto &component: components) {
        component->entityId = id;
    }
}

std::vector<std::unique_ptr<IComponent>> &&GameObject::getAllComponents() {
    return std::move(components);
}

std::vector<std::unique_ptr<IBehaviourScript>> &&GameObject::getAllBehaviourScripts() {
    return std::move(behaviourScripts);
}


void GameObject::addChild(std::unique_ptr<GameObject> child) {
    try {
        auto &childComponent = tryGetComponent<ChildComponent>();
        EntityManager::getInstance().setEntityActive(child->getEntityId(), isActive());

        if (entityID != 0 && child->getEntityId() != 0)
            childComponent.children.push_back(child->getEntityId());
    } catch (std::runtime_error &e) {
        addComponent(std::make_unique<ChildComponent>());
        EntityManager::getInstance().setEntityActive(child->getEntityId(), isActive());
        if (entityID != 0 && child->getEntityId() != 0) {
            auto &childComponent = tryGetComponent<ChildComponent>();
            childComponent.children.push_back(child->getEntityId());
        }
    }

    try {
        auto &parentComponent = child->tryGetComponent<ParentComponent>();
        parentComponent.parentId = entityID;
    } catch (std::runtime_error &e) {
        child->addComponent(ParentComponent());
        auto &parentComponent = child->tryGetComponent<ParentComponent>();
        parentComponent.parentId = entityID;
    }

    child->parent = this;
    if (entityID == 0) {
        children.push_back(std::move(child));
        return;
    }
    GameObjectConverter::addGameObject(child.get());
}

void GameObject::setRotation(float rotation) const {
    if (entityID == 0) {
        tryGetComponent<TransformComponent>().rotation = rotation;
    } else
        ComponentStore::GetInstance().tryGetComponent<TransformComponent>(
                entityID).rotation = rotation;
}