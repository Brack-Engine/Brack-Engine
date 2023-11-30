//
// Created by jesse on 02/11/2023.
//

#include <Components/ObjectInfoComponent.hpp>
#include <Components/AIComponent.hpp>
#include <Helpers/KeyMap.hpp>
#include "BrackEngine.hpp"
#include "Systems/RenderingSystem.hpp"
#include "Logger.hpp"
#include "ConfigSingleton.hpp"
#include "Systems/InputSystem.hpp"
#include "FPSSingleton.hpp"
#include "Systems/MovementSystem.hpp"
#include "Systems/BehaviourScriptSystem.hpp"
#include "Systems/ClickSystem.hpp"
#include "Systems/AudioSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/AnimationSystem.hpp"

BrackEngine::BrackEngine(Config &&config) {
    ConfigSingleton::GetInstance().SetConfig(config);
    SystemManager::GetInstance().AddSystem(std::make_shared<InputSystem>());
    SystemManager::GetInstance().AddSystem(std::make_shared<ClickSystem>());
    SystemManager::GetInstance().AddSystem(std::make_shared<AudioSystem>());
    SystemManager::GetInstance().AddSystem(std::make_shared<BehaviourScriptSystem>());
    SystemManager::GetInstance().AddSystem(std::make_shared<MovementSystem>());
    SystemManager::GetInstance().AddSystem(std::make_shared<PhysicsSystem>());
    SystemManager::GetInstance().AddSystem(std::make_shared<AnimationSystem>());
    SystemManager::GetInstance().AddSystem(std::make_shared<RenderingSystem>());

    lastTime = std::chrono::high_resolution_clock::now();

    if (ConfigSingleton::GetInstance().ShowFPS())
        CreateFPS();
}

void BrackEngine::Run() {
    Logger::Debug("Updating systems");
    while (ConfigSingleton::GetInstance().IsRunning()) {
        FPSSingleton::GetInstance().Start();
        milliseconds deltaTime = GetDeltaTime();
        SystemManager::GetInstance().UpdateSystems(deltaTime);
        FPSSingleton::GetInstance().End();
        if (ConfigSingleton::GetInstance().ShowFPS())
            UpdateFPS(deltaTime);
    }

    SystemManager::GetInstance().CleanUp();
}

milliseconds BrackEngine::GetDeltaTime() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(
            currentTime - lastTime);
    lastTime = currentTime;

    float deltaTimeInSeconds = deltaTime.count();
    milliseconds deltaTimeInMilliSeconds = deltaTimeInSeconds * 1000.0f;
    return deltaTimeInMilliSeconds;
}


void BrackEngine::CreateFPS() {
    auto entityId = EntityManager::getInstance().createEntity();
    auto objectInfoComponent = ObjectInfoComponent();
    auto textComponent = TextComponent();

    objectInfoComponent.name = "FPS";
    objectInfoComponent.tag = "FPS";
    objectInfoComponent.entityID = entityId;

    textComponent.text = "0";
    textComponent.fontSize = 32;
    textComponent.color = std::make_unique<Color>(255, 0, 0, 255);
    textComponent.entityID = entityId;

    ComponentStore::GetInstance().addComponent<TransformComponent>(entityId);
    ComponentStore::GetInstance().addComponent<ObjectInfoComponent>(objectInfoComponent);
    ComponentStore::GetInstance().addComponent<TextComponent>(textComponent);

    EntityManager::getInstance().addEntityWithName(entityId, objectInfoComponent.name);
    EntityManager::getInstance().addEntityWithTag(entityId, objectInfoComponent.tag);
}

void BrackEngine::UpdateFPS(float deltaTime) {
    totalTime += deltaTime;
    if (totalTime < 1 / 12.0f)
        return;

    totalTime = 0;
    auto &textComponent = ComponentStore::GetInstance().tryGetComponent<TextComponent>(
            1);//TODO ophalen met tag of name van component

    textComponent.text = std::to_string(FPSSingleton::GetInstance().GetFPS());;
}

void BrackEngine::save(const std::string &filePath, const std::string &content) const {
    std::ofstream file(filePath, std::ios::binary);

    // Check if the file can be opened for writing
    if (!file.is_open()) {
        std::cerr << "Unable to open file for writing: " << filePath << std::endl;
        return;
    }

    // Write the size of the string first
    size_t size = content.size();
    if (!file.write(reinterpret_cast<const char *>(&size), sizeof(size_t))) {
        std::cerr << "Error writing size to file: " << filePath << std::endl;
        file.close(); // Close the file before returning
        return;
    }

    // Write the content of the string
    if (!file.write(content.c_str(), size)) {
        std::cerr << "Error writing content to file: " << filePath << std::endl;
    }

    file.close();
}

std::string BrackEngine::load(const std::string &filePath) const {
    std::ifstream file(filePath, std::ios::binary);

    // Check if the file can be opened for reading
    if (!file.is_open()) {
        std::cerr << "Unable to open file for reading: " << filePath << std::endl;
        return "";
    }

    // Read the size of the string first
    size_t size;
    if (!file.read(reinterpret_cast<char *>(&size), sizeof(size_t))) {
        std::cerr << "Error reading size from file: " << filePath << std::endl;
        return "";
    }

    // Read the content of the string
    std::string content(size, '\0');
    if (!file.read(&content[0], size)) {
        std::cerr << "Error reading content from file: " << filePath << std::endl;
    }

    file.close();
    return content;
}