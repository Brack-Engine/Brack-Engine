//
// Created by jesse on 02/11/2023.
//

#ifndef BRACK_ENGINE_BRACKENGINE_HPP
#define BRACK_ENGINE_BRACKENGINE_HPP


#include <memory>
#include <chrono>
#include "../src/includes/EntityManager.hpp"
#include "../src/includes/SystemManager.hpp"
#include "EngineManagers/SceneManager.hpp"
#include "EngineManagers/InputManager.hpp"
#include "Config.hpp"
#include "../src/includes/ComponentStore.hpp"

class BrackEngine {
public:
    BrackEngine(std::unique_ptr<Config> config);

    ~BrackEngine() = default;

    void Run();

    void save(const std::string &filePath, const std::string &content) const;

    std::string load(const std::string &filePath) const;

private:
    milliseconds GetDeltaTime();

    void CreateFPS();

    void UpdateFPS(float deltaTime);

    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
    float totalTime = 0;
};


#endif //BRACK_ENGINE_BRACKENGINE_HPP
