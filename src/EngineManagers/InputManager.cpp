//
// Created by jesse on 02/11/2023.
//

#include <Helpers/KeyMap.hpp>
#include <Helpers/MouseMap.hpp>
#include "InputManager.hpp"
#include "../Logger.hpp"

InputManager InputManager::instance;

void InputManager::SetKeyPressed(const int key) {
    if(keyInputs.find(key)->second == None || keyInputs.find(key)->second == Released){
        keyInputs.find(key)->second = Clicked;
    }
}

void InputManager::SetKeyReleased(const int key) {
    keyInputs.find(key)->second = Released;
}

bool InputManager::IsKeyPressed(const int key) const {
    return keyInputs.find(key)->second == Pressed;
}

bool InputManager::IsKeyClicked(const int key) const {
    return keyInputs.find(key)->second == Clicked;
}

bool InputManager::IsKeyReleased(const int key) const {
    return keyInputs.find(key)->second == Released;
}

void InputManager::SetMousePressed(const int key) {
    mouseInputs.find(key)->second = Clicked;
}

void InputManager::SetMouseReleased(const int key) {
    mouseInputs.find(key)->second = Released;
}

bool InputManager::IsMousePressed(const int key) const {
    return mouseInputs.find(key)->second == Pressed;
}

bool InputManager::IsMouseClicked(const int key) const {
    return mouseInputs.find(key)->second == Clicked;
}

bool InputManager::IsMouseReleased(const int key) const {
    return mouseInputs.find(key)->second == Released;
}

void InputManager::SetMousePosition(const Vector2 &position) {
    mousePosition.reset();
    mousePosition = std::make_unique<Vector2>(position);
}

Vector2 &InputManager::GetMousePosition() const {
    return *mousePosition;
}

InputManager &InputManager::GetInstance() {
    return instance;
}

InputManager::InputManager() {
    mousePosition = std::make_unique<Vector2>(0,0);
    mouseInputs.emplace(LEFT_MOUSE,None);
    mouseInputs.emplace(MIDDLE_MOUSE,None);
    mouseInputs.emplace(RIGHT_MOUSE,None);

    for (KeyMap key : {KeyMap_List})
    {
        keyInputs.emplace(key, None);
    }
}

void InputManager::UpdateEvents() {
    for (auto& item : keyInputs) {
        if(item.second == Clicked){
            item.second = Pressed;
        }else if(item.second == Released){
            item.second = None;
        }
    }

    for (auto& item : mouseInputs) {
        if(item.second == Clicked){
            item.second = Pressed;
        }else if(item.second == Released){
            item.second = None;
        }
    }
}
