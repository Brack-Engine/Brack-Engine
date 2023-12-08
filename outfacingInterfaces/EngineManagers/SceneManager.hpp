//
// Created by jesse on 02/11/2023.
//

#ifndef BRACK_ENGINE_SCENEMANAGER_HPP
#define BRACK_ENGINE_SCENEMANAGER_HPP


#include <memory>
#include "../Components/TransformComponent.hpp"
#include "../../src/includes/EntityManager.hpp"
#include "../../src/GameObjectConverter.hpp"

class Scene;

class SceneManager {
public:
    static SceneManager &getInstance();

    ~SceneManager() = default;

    SceneManager(const SceneManager &) = delete;

    SceneManager &operator=(const SceneManager &) = delete;

    SceneManager(SceneManager &&) = delete;

    SceneManager &operator=(SceneManager &&) = delete;

    void setActiveScene(Scene &scene);

    std::optional<GameObject> getGameObjectByName(const std::string &name);

    std::optional<GameObject> getGameObjectByTag(const std::string &tag);

    std::vector<GameObject *> getGameObjectsByTag(const std::string &tag);

    Vector2 getWorldPosition(const TransformComponent &transformComponent);

    std::string getActiveSceneSignature();

    Scene &getActiveScene();

private:
    SceneManager() = default;

    static SceneManager instance;
    bool hasChanged = false;

    std::optional<std::reference_wrapper<Scene>> activeScene;
    std::string activeSceneSignature;

};


#endif //BRACK_ENGINE_SCENEMANAGER_HPP
