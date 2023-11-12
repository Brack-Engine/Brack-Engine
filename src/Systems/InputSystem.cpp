//
// Created by jesse on 31/10/2023.
//

#include "InputSystem.hpp"
#include "../EngineManagers/InputManager.hpp"

InputSystem::InputSystem() {
    inputWrapper = std::make_unique<InputWrapper>();
}

InputSystem::~InputSystem() {
    inputWrapper.reset();
}

void InputSystem::Update(float deltaTime) {
    InputManager::GetInstance().UpdateEvents();
    inputWrapper->HandleEvents();
}

const std::string InputSystem::GetName() const {
    return "InputSystem";
}

void InputSystem::CleanUp() {

}
