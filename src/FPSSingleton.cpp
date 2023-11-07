//
// Created by jesse on 07/11/2023.
//

#include "FPSSingleton.hpp"

FPSSingleton FPSSingleton::instance;

FPSSingleton &FPSSingleton::GetInstance() {
    return instance;
}

int FPSSingleton::GetFPS() const {
    return fpsWrapper.GetFPS();
}

void FPSSingleton::Start() {
    fpsWrapper.Start();
}

void FPSSingleton::End() {
    fpsWrapper.End();
}

FPSSingleton::FPSSingleton() {
    fpsWrapper = FPSWrapper();
}
