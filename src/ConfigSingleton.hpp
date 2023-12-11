//
// Created by jesse on 06/11/2023.
//

#ifndef BRACKOCALYPSE_CONFIGSINGLETON_HPP
#define BRACKOCALYPSE_CONFIGSINGLETON_HPP


#include <string>
#include <Helpers/Vector2.hpp>
#include <Config.hpp>

class ConfigSingleton {
public:
    static ConfigSingleton &GetInstance();

    ConfigSingleton(ConfigSingleton &other) = delete;

    void operator=(const ConfigSingleton &) = delete;

    ConfigSingleton(ConfigSingleton &&other) = delete;

    void operator=(ConfigSingleton &&) = delete;

    void SetConfig(std::unique_ptr<Config> config);

    void SetIsRunning(bool isRunning);

    bool IsRunning() const;

    bool ShowFPS() const;

    std::string GetWindowTitle() const;

    Vector2 GetWindowSize() const;

    void SetWindowSize(Vector2 size) const;

    bool IsFullscreen() const;

    int getParticleLimit() const;

    std::string GetBaseAssetPath() const;

    int getAmountOfSoundEffectsChannels() const;

    uint32_t GetFPSLimit() const;

    void ToggleIsRunning();

private:
    static ConfigSingleton instance;

    ConfigSingleton() = default;

    bool isRunning = true;
    std::string windowTitle = "Brack-Engine";
    Vector2 windowSize = Vector2(800, 600);
    bool fullscreen = false;
    std::string BaseAssetPath = "./Assets/";
    bool showFPS = true;
    int amountOfSoundEffectsChannels = 5;
    uint32_t fpsLimit = 60;
    int particleLimit = 100;
};


#endif //BRACKOCALYPSE_CONFIGSINGLETON_HPP
