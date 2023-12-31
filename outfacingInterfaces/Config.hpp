//
// Created by jesse on 06/11/2023.
//

#ifndef BRACKOCALYPSE_CONFIG_HPP
#define BRACKOCALYPSE_CONFIG_HPP

#include <string>
#include <cstdint>
#include "Helpers/Vector2.hpp"

class Config {
public:
    Config();

    Config(bool isRunning);

    bool isRunning = true;
    std::string windowTitle = "Brack-Engine";
    Vector2 windowSize = Vector2(0, 0);
    Vector2 initialWindowSize = Vector2(1280, 720);
    bool fullscreen = false;
    std::string BaseAssetPath = "./Assets/";
    std::string appLogoPath = "";
    bool showFPS = true;
    int amountOfSoundEffectsChannels = 7;
    uint16_t fpsLimit = 0;
    int particleLimit = 100;
    float deltaTimeMultiplier = 1.0;
};


#endif //BRACKOCALYPSE_CONFIG_HPP
