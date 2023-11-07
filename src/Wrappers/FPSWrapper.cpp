//
// Created by jesse on 07/11/2023.
//

#include "FPSWrapper.hpp"

void FPSWrapper::Start() {
    start = SDL_GetPerformanceCounter();
}

void FPSWrapper::End() {
    end = SDL_GetPerformanceCounter();
    float elapsed = (end - start) / (float) SDL_GetPerformanceFrequency();
    fps = (int) (1.0f / elapsed);
}

int FPSWrapper::GetFPS() const {
    return fps;
}
