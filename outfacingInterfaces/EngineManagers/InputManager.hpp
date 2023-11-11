//
// Created by jesse on 02/11/2023.
//

#ifndef BRACK_ENGINE_INPUTMANAGER_HPP
#define BRACK_ENGINE_INPUTMANAGER_HPP

#include <map>
#include <memory>
#include "Helpers/Vector2.hpp"
#include "Helpers/InputState.hpp"
#include "../../src/includes/EntityManager.hpp"

class InputManager {
public:
    static InputManager &GetInstance();

    ~InputManager() = default;

    InputManager(const InputManager &) = delete;

    InputManager &operator=(const InputManager &) = delete;

    InputManager &operator=(InputManager &&) = delete;

    InputManager(InputManager &&) = delete;

    void SetKeyPressed(const int key);

    void SetKeyReleased(const int key);

    bool IsKeyPressed(const int key) const;

    bool IsKeyClicked(const int key) const;

    bool IsKeyReleased(const int key) const;

    void SetMousePressed(const int key);

    void SetMouseReleased(const int key);

    bool IsMousePressed(const int key) const;

    bool IsMouseClicked(const int key) const;

    bool IsMouseReleased(const int key) const;

    void SetMousePosition(const Vector2 &position);

    Vector2 &GetMousePosition() const;

    void UpdateEvents();

private:
    InputManager();

    static InputManager instance;

    std::map<int, InputState> keyInputs;
    std::map<int, InputState> mouseInputs;
    std::unique_ptr<Vector2> mousePosition;
};


#endif //BRACK_ENGINE_INPUTMANAGER_HPP
