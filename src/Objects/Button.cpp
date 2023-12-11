//
// Created by jesse on 31/10/2023.
//

#include <Components/RectangleComponent.hpp>
#include <Components/TextComponent.hpp>
#include <Components/ClickableComponent.hpp>
#include <Components/BoxCollisionComponent.hpp>
#include <utility>
#include <Components/RigidBodyComponent.hpp>
#include "Objects/Button.hpp"

Button::Button(const Vector2 &size, const std::string &text) : GameObject() {
    auto textComponent = std::make_unique<TextComponent>();
    textComponent->text = std::move(text);
    textComponent->orderInLayer = 0;
    textComponent->sortingLayer = 0;

    auto rectangleComponent = std::make_unique<RectangleComponent>(size);
    rectangleComponent->orderInLayer = 1;
    rectangleComponent->sortingLayer = 0;

    auto boxCollisionComponent = std::make_unique<BoxCollisionComponent>(size);

    auto clickableComponent = std::make_unique<ClickableComponent>();

    addComponent(std::move(boxCollisionComponent));
    addComponent(std::make_unique<RigidBodyComponent>());
    addComponent(std::move(clickableComponent));
    addComponent(std::move(rectangleComponent));
    addComponent(std::move(textComponent));
}

void Button::setTextColor(const Color &color) {
    if (entityID == 0) {
        tryGetComponent<TextComponent>().color = std::make_unique<Color>(color);
    } else
        ComponentStore::GetInstance().tryGetComponent<TextComponent>(entityID).color = std::make_unique<Color>(color);
}

void Button::setFontPath(const std::string &path) {
    if (entityID == 0) {
        tryGetComponent<TextComponent>().fontPath = path;
    } else
        ComponentStore::GetInstance().tryGetComponent<TextComponent>(entityID).fontPath = path;
}

void Button::setFontSize(int fontSize) {
    if (entityID == 0) {
        tryGetComponent<TextComponent>().fontSize = fontSize;
    } else
        ComponentStore::GetInstance().tryGetComponent<TextComponent>(entityID).fontSize = fontSize;
}

void Button::setText(const std::string &string) {
    if (entityID == 0) {
        tryGetComponent<TextComponent>().text = string;
    } else
        ComponentStore::GetInstance().tryGetComponent<TextComponent>(entityID).text = string;
}

void Button::setFill(const Color &color) {
    if (entityID == 0) {
        tryGetComponent<RectangleComponent>().fill = std::make_unique<Color>(color);
    } else
        ComponentStore::GetInstance().tryGetComponent<RectangleComponent>(entityID).fill = std::make_unique<Color>(
                color);
}

void Button::setClickEvent(std::function<void()> func) {
    tryGetComponent<ClickableComponent>().OnClick = std::move(func);
}

bool Button::isDisabled() const {
    return tryGetComponent<ClickableComponent>().disabled;
}

void Button::setDisabled(bool disabled) {
    if (entityID == 0) {
        tryGetComponent<ClickableComponent>().disabled = disabled;
    } else
        ComponentStore::GetInstance().tryGetComponent<ClickableComponent>(entityID).disabled = disabled;
}
