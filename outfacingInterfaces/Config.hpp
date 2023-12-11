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
    Vector2 windowSize = Vector2(800, 600);
    bool fullscreen = false;
    std::string BaseAssetPath = "./Assets/";
    bool showFPS = true;
    int amountOfSoundEffectsChannels = 5;
    uint16_t fpsLimit = 0;
    int particleLimit = 100;
};


#endif //BRACKOCALYPSE_CONFIG_HPP
