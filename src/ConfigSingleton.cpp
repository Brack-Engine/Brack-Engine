//
// Created by jesse on 06/11/2023.
//

#include <EngineManagers/SceneManager.hpp>
#include "ConfigSingleton.hpp"
#include "includes/EntityManager.hpp"

ConfigSingleton ConfigSingleton::instance;

ConfigSingleton &ConfigSingleton::GetInstance() {
    return instance;
}

bool ConfigSingleton::IsRunning() const {
    return isRunning;
}

void ConfigSingleton::ToggleIsRunning() {
    isRunning = !isRunning;
}

void ConfigSingleton::setDeltaTimeMultiplier(float deltaTimeMultiplier) {
    ConfigSingleton::instance.deltaTimeMultiplier = deltaTimeMultiplier;
}

void ConfigSingleton::SetIsRunning(bool isRunning) {
    ConfigSingleton::isRunning = isRunning;
}

std::string ConfigSingleton::GetWindowTitle() const {
    return windowTitle;
}

Vector2 ConfigSingleton::GetWindowSize() const {
    return windowSize;
}

bool ConfigSingleton::IsFullscreen() const {
    return fullscreen;
}

int ConfigSingleton::getParticleLimit() const {
    return particleLimit;
}

std::string ConfigSingleton::GetBaseAssetPath() const {
    return BaseAssetPath;
}

int ConfigSingleton::getAmountOfSoundEffectsChannels() const {
    return amountOfSoundEffectsChannels;
}

void ConfigSingleton::SetConfig(Config config) {
    isRunning = config.isRunning;
    windowTitle = config.windowTitle;
    windowSize = config.windowSize;
    fullscreen = config.fullscreen;
    BaseAssetPath = config.BaseAssetPath;
    showFPS = config.showFPS;
    amountOfSoundEffectsChannels = config.amountOfSoundEffectsChannels;
    fpsLimit = config.fpsLimit;
    particleLimit = config.particleLimit;
}

bool ConfigSingleton::IsShowFPSTrue() const {
    return showFPS;
}

void ConfigSingleton::toggleShowFps() {
    showFPS = !showFPS;
    if (EntityManager::getInstance().entityExistsByTag("FPS")) {
        SceneManager::getInstance().getGameObjectByName("FPS").value()->setActive(showFPS);
    }
}

void ConfigSingleton::SetWindowSize(Vector2 size) const {
    size = size;
}

uint32_t ConfigSingleton::GetFPSLimit() const {
    return fpsLimit;
}

void ConfigSingleton::setFPSLimit(uint32_t fpsLimit) {
    ConfigSingleton::fpsLimit = fpsLimit;
}
