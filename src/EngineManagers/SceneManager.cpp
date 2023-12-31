//
// Created by jesse on 02/11/2023.
//

#include <Components/ChildComponent.hpp>
#include <Components/ParentComponent.hpp>
#include <EngineManagers/ReplayManager.hpp>
#include "EngineManagers/SceneManager.hpp"
#include "Objects/Scene.hpp"
#include "../GameObjectConverter.hpp"
#include "../../src/includes/SystemManager.hpp"
#include "../FPSSingleton.hpp"
#include "ConfigSingleton.hpp"

SceneManager SceneManager::instance;

void SceneManager::setActiveScene() {
    if (switchingScene == nullptr)
        return;

    EntityManager::getInstance().clearAllEntities();
    SystemManager::getInstance().clearSystemsCache();

    for (auto camera: switchingScene->getAllCameras())
        GameObjectConverter::addGameObject(camera);

    for (auto gameObject: switchingScene->getAllGameObjects()) {
        GameObjectConverter::addGameObject(gameObject);
    }

    activeSceneSignature = switchingScene->getSignature();
    delete switchingScene;
    switchingScene = nullptr;
}

SceneManager &SceneManager::getInstance() {
    return instance;
}

std::string SceneManager::getActiveSceneSignature() {
    return activeSceneSignature;
}

std::vector<GameObject> SceneManager::getGameObjectsByName(const std::string &name) {
    return GameObjectConverter::getGameObjectsByName(name);
}

std::optional<GameObject *> SceneManager::getGameObjectByName(const std::string &name) {
    return GameObjectConverter::getGameObjectByName(name);
}

std::vector<GameObject *> SceneManager::getGameObjectsByTag(const std::string &tag) {
    return GameObjectConverter::getGameObjectsByTag(tag);
}

Vector2 SceneManager::getWorldPosition(const TransformComponent &transformComponent) {
    auto position = *transformComponent.position;
    try {
        auto parentId = ComponentStore::GetInstance().tryGetComponent<ParentComponent>(
            transformComponent.entityId).parentId;
        auto &parentTransform = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(parentId);
        position += getWorldPosition(parentTransform);
        return position;
    } catch (std::runtime_error &e) {
        return position;
    }
}

Vector2 SceneManager::getWorldScale(const TransformComponent &transformComponent) {
    auto scale = *transformComponent.scale;

    try {
        auto parentId = ComponentStore::GetInstance().tryGetComponent<ParentComponent>(
            transformComponent.entityId).parentId;
        auto &parentTransform = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(parentId);
        scale *= getWorldScale(parentTransform);
        return scale;
    } catch (std::runtime_error &e) {
        return scale;
    }
}

float SceneManager::getWorldRotation(const TransformComponent &transformComponent) {
    auto rotation = transformComponent.rotation;

    try {
        auto parentId = ComponentStore::GetInstance().tryGetComponent<ParentComponent>(
            transformComponent.entityId).parentId;
        auto &parentTransform = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(parentId);
        rotation += getWorldRotation(parentTransform);
        return rotation;
    } catch (std::runtime_error &e) {
        return rotation;
    }
}

Vector2 SceneManager::getLocalPosition(const Vector2 worldPosition, entity entityId) {
    auto position = worldPosition;
    try {
        auto parentId = ComponentStore::GetInstance().tryGetComponent<ParentComponent>(entityId).parentId;
        auto &parentTransform = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(parentId);
        position -= getWorldPosition(parentTransform);
        return position;
    } catch (std::runtime_error &e) {
        return position;
    }
}


void SceneManager::goToNewScene(Scene *scene) {
    switchingScene = scene;
}

void SceneManager::toggleFPS() {
    ConfigSingleton::getInstance().toggleShowFps();
}
