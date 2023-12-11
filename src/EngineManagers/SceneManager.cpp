//
// Created by jesse on 02/11/2023.
//

#include <Components/ChildComponent.hpp>
#include <Components/ParentComponent.hpp>
#include "EngineManagers/SceneManager.hpp"
#include "Objects/Scene.hpp"
#include "../GameObjectConverter.hpp"
#include "../../src/includes/SystemManager.hpp"

SceneManager SceneManager::instance;

void SceneManager::setActiveScene(Scene &scene) {
    EntityManager::getInstance().clearAllEntities();
    SystemManager::getInstance().clearSystemsCache();

    for (auto &camera: scene.getAllCameras())
        GameObjectConverter::addGameObject(reinterpret_cast<std::unique_ptr<GameObject> &>(camera));

    for (auto &gameObject: scene.getAllGameObjects())
        GameObjectConverter::addGameObject(gameObject);

    activeSceneSignature = scene.getSignature();
    activeScene = scene;
}

SceneManager &SceneManager::getInstance() {
    return instance;
}

Scene &SceneManager::getActiveScene() {
    return activeScene.value();
}

std::string SceneManager::getActiveSceneSignature() {
    return activeSceneSignature;
}

std::vector<GameObject *> SceneManager::getGameObjectsByTag(const std::string &tag) {
    return activeScene->get().getGameObjectsByTag(tag);
}

Vector2 SceneManager::getWorldPosition(const TransformComponent &transformComponent) {
    auto position = *transformComponent.position;
    try {
        auto parentId = ComponentStore::GetInstance().tryGetComponent<ParentComponent>(
                transformComponent.entityID).parentId;
        auto &parentTransform = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(parentId);
        position += getWorldPosition(parentTransform);
        return position;
    }
    catch (std::runtime_error &e) {
        return position;
    }
}

Vector2 SceneManager::getWorldScale(const TransformComponent &transformComponent) {
    auto scale = *transformComponent.scale;

    try {
        auto parentId = ComponentStore::GetInstance().tryGetComponent<ParentComponent>(
                transformComponent.entityID).parentId;
        auto &parentTransform = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(parentId);
        scale *= getWorldScale(parentTransform);
        return scale;
    }
    catch (std::runtime_error &e) {
        return scale;
    }
}

float SceneManager::getWorldRotation(const TransformComponent &transformComponent) {
    auto rotation = transformComponent.rotation;

    try {
        auto parentId = ComponentStore::GetInstance().tryGetComponent<ParentComponent>(
                transformComponent.entityID).parentId;
        auto &parentTransform = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(parentId);
        rotation += getWorldRotation(parentTransform);
        return rotation;
    }
    catch (std::runtime_error &e) {
        return rotation;
    }
}


