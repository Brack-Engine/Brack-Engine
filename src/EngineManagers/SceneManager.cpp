//
// Created by jesse on 02/11/2023.
//

#include <iostream>
#include "SceneManager.hpp"
#include "Objects/Scene.hpp"
#include "../GameObjectConverter.hpp"

SceneManager SceneManager::instance;

void SceneManager::SetActiveScene(Scene &scene) {
    EntityManager::GetInstance().ClearAllEntities();
    auto camera = scene.GetMainCamera();
    GameObjectConverter::AddCamera(camera);

    for (auto &gameObject : scene.GetAllGameObjects()) {
        GameObjectConverter::AddGameObject(gameObject);
    }
}




SceneManager &SceneManager::GetInstance() {
    return instance;
}
