//
// Created by jesse on 31/10/2023.
//

#include <Components/TransformComponent.hpp>
#include <Components/ObjectInfoComponent.hpp>
#include <Components/ParentComponent.hpp>
#include <Components/ChildComponent.hpp>
#include "Objects/GameObject.hpp"
#include <algorithm>
#include <utility>
#include <optional>

GameObject::GameObject() {
    addComponent(std::make_unique<TransformComponent>());
    addComponent(std::make_unique<ObjectInfoComponent>());
}

std::vector<GameObject *> GameObject::getChildren() const {
    if (entityID == 0) {
        return children;
    }
    try {
        auto &childComponent = tryGetComponent<ChildComponent>();
        std::vector<GameObject *> childrenFromComponent;
        for (auto &childId: childComponent.children) {
            auto *child = new GameObject();
            child->setEntityId(childId);
            childrenFromComponent.push_back(child);
        }
        return childrenFromComponent;
    } catch (std::runtime_error &e) {
        return {};
    }
}

std::optional<GameObject> GameObject::getParent() {
    if (parent == nullptr)
        return std::nullopt;
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
    return tryGetComponent<ObjectInfoComponent>().isActive;
}

void GameObject::setActive(bool active) const {
    tryGetComponent<ObjectInfoComponent>().isActive = active;
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
        component->entityID = id;
    }
}

std::vector<std::unique_ptr<IComponent>> &&GameObject::getAllComponents() {
    return std::move(components);
}

void GameObject::addChild(GameObject &child) {
    try {
        auto &childComponent = tryGetComponent<ChildComponent>();
        if (entityID != 0)
            childComponent.children.push_back(child.getEntityId());
    } catch (std::runtime_error &e) {
        addComponent(std::make_unique<ChildComponent>());
        if (entityID != 0) {
            auto &childComponent = tryGetComponent<ChildComponent>();
            childComponent.children.push_back(child.getEntityId());
        }
    }

    try {
        auto &parentComponent = child.tryGetComponent<ParentComponent>();
        parentComponent.parentId = entityID;
    } catch (std::runtime_error &e) {
        child.addComponent(ParentComponent());
        auto &parentComponent = child.tryGetComponent<ParentComponent>();
        parentComponent.parentId = entityID;
    }

    child.parent = this;
    children.emplace_back(&child);
}

void GameObject::setRotation(float rotation) const {
    if (entityID == 0) {
        tryGetComponent<TransformComponent>().rotation = rotation;
    } else
        ComponentStore::GetInstance().tryGetComponent<TransformComponent>(
                entityID).rotation = rotation;
}

void GameObject::removeChild(GameObject &child) {
    try {
        auto &childComponent = tryGetComponent<ChildComponent>();
        auto it = std::find(childComponent.children.begin(), childComponent.children.end(),
                            child.getEntityId());
        if (it != childComponent.children.end()) {
            childComponent.children.erase(it);
            std::remove(children.begin(), children.end(), (&child));
            child.parent = nullptr;
            if (childComponent.children.empty()) {
                removeComponent<ChildComponent>();
            }
        }

        child.removeComponent<ParentComponent>();
    } catch (std::runtime_error &e) {
        // Do nothing
    }

}