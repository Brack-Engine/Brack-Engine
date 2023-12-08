//
// Created by jesse on 31/10/2023.
//

#include <algorithm>
#include "Objects/Scene.hpp"

void Scene::addGameObject(std::unique_ptr<GameObject> gameObject) {
    if (SceneManager::getInstance().getActiveSceneSignature() == signature) {
        GameObjectConverter::addGameObject(gameObject);
        gameObjects.push_back(std::move(gameObject));
    } else
        gameObjects.push_back(std::move(gameObject));

    if (!gameObjects.back()->getName().empty())
        nameToGameObject.insert({gameObjects.back()->getName(), gameObjects.back()});
    if (!gameObjects.back()->getTag().empty()) {
        if (tagToGameObject.count(gameObjects.back()->getTag()) > 0) {
            tagToGameObject.find(gameObjects.back()->getTag())->second.push_back(gameObjects.back());
        } else {
            tagToGameObject.insert(
                    {gameObjects.back()->getTag(), std::vector<std::reference_wrapper<std::unique_ptr<GameObject>>>()});
            tagToGameObject.find(gameObjects.back()->getTag())->second.push_back(gameObjects.back());
        }
    }
}

std::unique_ptr<GameObject> &Scene::getGameObjectByName(const std::string name) {
    if (nameToGameObject.find(name) != nameToGameObject.end())
        return nameToGameObject.find(name)->second;

    throw std::runtime_error("No GameObject with name " + std::string(name) + " found.");
}

std::vector<GameObject *> Scene::getGameObjectsByTag(const std::string &tag) {
    std::vector<GameObject *> gameObjectsWithTag;
    for (auto &gameObject: gameObjects) {
        if (gameObject->getTag() == tag) {
            gameObjectsWithTag.push_back(gameObject.get());
        }
    }

    return gameObjectsWithTag;
}

std::vector<std::unique_ptr<GameObject>> &Scene::getAllGameObjects() {
    return gameObjects;
}

std::vector<std::unique_ptr<Camera>> &Scene::getAllCameras() {
    return cameras;
}

void Scene::removeGameObjectByName(std::string name) {
    auto &gameObject = getGameObjectByName(name);

    removeGameObject(gameObject);
}

void Scene::removeGameObject(std::unique_ptr<GameObject> &gameObject) {
    std::string name = gameObject->getName();
    std::string tag = gameObject->getTag();

    if (SceneManager::getInstance().getActiveSceneSignature() == signature) {
        GameObjectConverter::removeGameObject(gameObject);
    }

    auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
                           [&gameObject](const std::unique_ptr<GameObject> &ptr) {
                               return ptr == gameObject;
                           });
    if (it != gameObjects.end()) {
        gameObjects.erase(it);
    }

    if (!name.empty())
        nameToGameObject.erase(name);

    if (!tag.empty()) {
        auto &gameObjectsWithTag = tagToGameObject.find(tag)->second;
        auto it = std::find_if(gameObjectsWithTag.begin(), gameObjectsWithTag.end(),
                               [&gameObject](const std::reference_wrapper<std::unique_ptr<GameObject>> &ptr) {
                                   return ptr.get() == gameObject;
                               });
        if (it != gameObjectsWithTag.end()) {
            gameObjectsWithTag.erase(it);
        }
    }
}

Scene::Scene(Camera &&mainCamera) {
    addCamera(std::move(mainCamera));
    signature = generateSignature();
}

Scene::Scene() {
    addCamera(Camera());
    signature = generateSignature();
}

std::vector<std::unique_ptr<Camera>> &Scene::getCameras() {
    return cameras;
}

void Scene::addCamera(Camera &&camera) {
    cameras.push_back(std::make_unique<Camera>(camera));
}

std::string Scene::getSignature() {
    return signature;
}

std::string Scene::generateSignature() {
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    std::string random_string;

    for (std::size_t i = 0; i < 10; ++i) {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}
