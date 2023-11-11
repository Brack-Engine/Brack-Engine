//
// Created by jesse on 02/11/2023.
//

#ifndef BRACK_ENGINE_SCENEMANAGER_HPP
#define BRACK_ENGINE_SCENEMANAGER_HPP


#include <memory>
#include "../../src/includes/EntityManager.hpp"

class Scene;

class SceneManager {
public:
    static SceneManager &GetInstance();

    ~SceneManager() = default;

    SceneManager(const SceneManager &) = delete;

    SceneManager &operator=(const SceneManager &) = delete;

    SceneManager(SceneManager &&) = delete;

    SceneManager &operator=(SceneManager &&) = delete;

    void SetActiveScene(Scene &scene);

private:
    SceneManager() = default;

    static SceneManager instance;
    bool hasChanged = false;

};


#endif //BRACK_ENGINE_SCENEMANAGER_HPP
