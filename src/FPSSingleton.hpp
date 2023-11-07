//
// Created by jesse on 07/11/2023.
//

#ifndef BRACKOCALYPSE_FPSSINGLETON_HPP
#define BRACKOCALYPSE_FPSSINGLETON_HPP


#include "Wrappers/FPSWrapper.hpp"

class FPSSingleton {
public:
    static FPSSingleton &GetInstance();

    FPSSingleton(FPSSingleton &other) = delete;

    void operator=(const FPSSingleton &) = delete;

    FPSSingleton(FPSSingleton &&other) = delete;

    void operator=(FPSSingleton &&) = delete;

    void Start();

    void End();

    int GetFPS() const;

private:
    static FPSSingleton instance;

    FPSSingleton();

    FPSWrapper fpsWrapper;
};


#endif //BRACKOCALYPSE_FPSSINGLETON_HPP
