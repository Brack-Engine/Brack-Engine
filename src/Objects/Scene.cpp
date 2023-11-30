//
// Created by jesse on 31/10/2023.
//

#include <algorithm>
#include "Objects/Scene.hpp"

void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject) {
    gameObjects.push_back(std::move(gameObject));
}

std::unique_ptr<GameObject> &Scene::GetGameObjectByName(char *name) {
    for (auto &gameObject: gameObjects) {
        if (gameObject->getName() == name)
            return gameObject;
    }

    throw std::runtime_error("No GameObject with name " + std::string(name) + " found.");
}

std::vector<GameObject *> Scene::GetGameObjectsByTag(const std::string &tag) {
    std::vector<GameObject *> gameObjectsWithTag;
    for (auto &gameObject: gameObjects) {
        if (gameObject->getTag() == tag) {
            gameObjectsWithTag.push_back(gameObject.get());
        }
    }

    return gameObjectsWithTag;
}

std::vector<GameObject *> Scene::GetAllGameObjects() {
    std::vector<GameObject *> gameObjectsPtrs;
    for (auto &gameObject: gameObjects) {
        gameObjectsPtrs.push_back(gameObject.get());
    }
    return gameObjectsPtrs;
}

std::vector<Camera *> Scene::GetAllCameras() {
    std::vector<Camera *> camerasPtrs;
    for (auto &camera: cameras) {
        camerasPtrs.push_back(camera.get());
    }
    return camerasPtrs;
}

void Scene::RemoveGameObject(GameObject &gameObject) {
    auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
                           [&gameObject](const std::unique_ptr<GameObject> &ptr) {
                               return ptr.get() == &gameObject;
                           });
    if (it != gameObjects.end()) {
        gameObjects.erase(it);
    }
}

Scene::Scene(Camera &&mainCamera) {
    AddCamera(std::move(mainCamera));
}

Scene::Scene() {
    AddCamera(Camera());

}

std::vector<std::unique_ptr<Camera>> &Scene::GetCameras() {
    return cameras;
}

void Scene::AddCamera(Camera &&camera) {
    cameras.push_back(std::make_unique<Camera>(camera));
}



