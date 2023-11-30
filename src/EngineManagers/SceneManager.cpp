//
// Created by jesse on 02/11/2023.
//

#include <Components/ChildComponent.hpp>
#include <Components/ParentComponent.hpp>
#include "EngineManagers/SceneManager.hpp"
#include "Objects/Scene.hpp"
#include "../GameObjectConverter.hpp"

SceneManager SceneManager::instance;

void SceneManager::setActiveScene(Scene &scene) {
    EntityManager::getInstance().clearAllEntities();

    for (auto camera: scene.GetAllCameras())
        GameObjectConverter::addGameObject(camera);

    for (auto gameObject: scene.GetAllGameObjects()) {
        GameObjectConverter::addGameObject(gameObject);
    }
}

SceneManager &SceneManager::getInstance() {
    return instance;
}

std::optional<GameObject> SceneManager::getGameObjectByName(const std::string &name) {
    return GameObjectConverter::getGameObjectByName(name);
}

std::vector<GameObject> SceneManager::getGameObjectsByName(const std::string &name) {
    return GameObjectConverter::getGameObjectsByName(name);
}

std::optional<GameObject> SceneManager::getGameObjectByTag(const std::string &tag) {
    return GameObjectConverter::getGameObjectByTag(tag);
}

std::vector<GameObject> SceneManager::getGameObjectsByTag(const std::string &tag) {
    return GameObjectConverter::getGameObjectsByTag(tag);
}
