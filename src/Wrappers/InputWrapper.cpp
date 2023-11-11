//
// Created by jesse on 31/10/2023.
//

#include <SDL.h>
#include "EngineManagers/InputManager.hpp"
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
    SDL_Event lastMouseMotionEvent{};
    SDL_Event lastMouseButtonDownEvent{};
    SDL_Event lastMouseButtonUpEvent{};
    SDL_Event lastKeyDownEvent{};
    SDL_Event lastKeyUpEvent{};
    // Close the window if the user presses the "X" button.
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            ConfigSingleton::GetInstance().ToggleIsRunning();
        }
        if (event.type == SDL_MOUSEMOTION) {
            lastMouseMotionEvent = event;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            lastMouseButtonDownEvent = event;
        }
        if (event.type == SDL_MOUSEBUTTONUP) {
            lastMouseButtonUpEvent = event;
        }
        if (event.type == SDL_KEYDOWN) {
            lastKeyDownEvent = event;
        }
        if (event.type == SDL_KEYUP) {
            lastKeyUpEvent = event;
        }
    }

    if (lastMouseMotionEvent.type != 0) {
        InputManager::GetInstance().SetMousePosition(
                Vector2(
                        static_cast<float>(lastMouseMotionEvent.motion.x),
                        static_cast<float>(lastMouseMotionEvent.motion.y)
                )
        );
    }
    if (lastMouseButtonDownEvent.type != 0) {
        InputManager::GetInstance().SetMousePressed(lastMouseButtonDownEvent.button.button);
    }
    if (lastMouseButtonUpEvent.type != 0) {
        InputManager::GetInstance().SetMouseReleased(lastMouseButtonUpEvent.button.button);
    }
    if (lastKeyDownEvent.type != 0) {
        InputManager::GetInstance().SetKeyPressed(lastKeyDownEvent.key.keysym.sym);
    }
    if (lastKeyUpEvent.type != 0) {
        InputManager::GetInstance().SetKeyReleased(lastKeyUpEvent.key.keysym.sym);
    }
}
