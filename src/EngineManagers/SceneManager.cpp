//
// Created by jesse on 02/11/2023.
//

#include <iostream>
#include "EngineManagers/SceneManager.hpp"
#include "Objects/Scene.hpp"
#include "../GameObjectConverter.hpp"

SceneManager SceneManager::instance;

void SceneManager::SetActiveScene(Scene &scene) {
    EntityManager::GetInstance().ClearAllEntities();
    auto cameras = scene.GetCameras();
    for (auto &camera: cameras)
        GameObjectConverter::AddCamera(camera);

    for (auto &gameObject: scene.GetAllGameObjects()) {
        GameObjectConverter::AddGameObject(gameObject);
    }
}


SceneManager &SceneManager::GetInstance() {
    return instance;
}
