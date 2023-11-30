//
// Created by jesse on 31/10/2023.
//

#include "Objects/Camera.hpp"
#include "../ConfigSingleton.hpp"

#include <memory>


Camera::Camera() : GameObject() {
    addComponent(std::make_unique<CameraComponent>());
    SetBackgroundColor(Color(0, 0, 0, 255));
    auto size = ConfigSingleton::GetInstance().GetWindowSize();
    auto halfSize = size / 2;
    SetOnScreenPosition(halfSize);
    SetSize(size);
}

void Camera::SetBackgroundColor(const Color &color) {
    if (entityID == 0) {
        tryGetComponent<CameraComponent>().backgroundColor = std::make_unique<Color>(color);
    } else
        ComponentStore::GetInstance().tryGetComponent<CameraComponent>(
                entityID).backgroundColor = std::make_unique<Color>(color);
}

void Camera::SetSize(const Vector2 &size) {
    if (entityID == 0) {
        tryGetComponent<CameraComponent>().size = std::make_unique<Vector2>(size);
    } else
        ComponentStore::GetInstance().tryGetComponent<CameraComponent>(entityID).size = std::make_unique<Vector2>(size);
}

void Camera::SetOnScreenPosition(const Vector2 &position) {
    if (entityID == 0) {
        tryGetComponent<CameraComponent>().onScreenPosition = std::make_unique<Vector2>(position);
    } else
        ComponentStore::GetInstance().tryGetComponent<CameraComponent>(
                entityID).onScreenPosition = std::make_unique<Vector2>(position);
}

