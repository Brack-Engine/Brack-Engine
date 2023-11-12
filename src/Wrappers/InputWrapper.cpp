//
// Created by jesse on 31/10/2023.
//

#include <SDL.h>
#include "../EngineManagers/InputManager.hpp"
#include "InputWrapper.hpp"
#include "../Logger.hpp"
#include "../ConfigSingleton.hpp"

InputWrapper::InputWrapper() {

}

InputWrapper::~InputWrapper() {

}

void InputWrapper::HandleEvents() {
    // Process events.
    SDL_Event event;
    SDL_PollEvent(&event);
    // Close the window if the user presses the "X" button.
    if (event.type == SDL_QUIT) {
        ConfigSingleton::GetInstance().ToggleIsRunning();
    }

    if (event.type == SDL_MOUSEMOTION){
        InputManager::GetInstance().SetMousePosition(
    Vector2(
            static_cast<float>(event.motion.x),
            static_cast<float>(event.motion.y)
            )
        );
    }

    if (event.type == SDL_MOUSEBUTTONDOWN){
        InputManager::GetInstance().SetMousePressed(event.button.button);
    }

    if (event.type == SDL_MOUSEBUTTONUP){
        InputManager::GetInstance().SetMouseReleased(event.button.button);
    }

    if (event.type == SDL_KEYDOWN){
        InputManager::GetInstance().SetKeyPressed(event.key.keysym.sym);
    }

    if (event.type == SDL_KEYUP){
        InputManager::GetInstance().SetKeyReleased(event.key.keysym.sym);
    }
}
