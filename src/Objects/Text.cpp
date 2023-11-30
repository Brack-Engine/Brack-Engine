//
// Created by jesse on 31/10/2023.
//

#include <Components/TextComponent.hpp>
#include <Components/RectangleComponent.hpp>
#include <utility>
#include "Objects/Text.hpp"
#include "../ConfigSingleton.hpp"

Text::Text(const std::string &text) : GameObject() {
    auto textComponent = std::make_unique<TextComponent>();
    textComponent->text = text;
    addComponent(std::move(textComponent));
}

void Text::SetFontSize(int fontSize) {
    if (entityID == 0) {
        tryGetComponent<TextComponent>().fontSize = fontSize;
    } else
        ComponentStore::GetInstance().tryGetComponent<TextComponent>(entityID).fontSize = fontSize;
}

void Text::SetColor(const Color &color) {
    if (entityID == 0) {
        tryGetComponent<TextComponent>().color = std::make_unique<Color>(color);
    } else
        ComponentStore::GetInstance().tryGetComponent<TextComponent>(entityID).color = std::make_unique<Color>(color);
}

void Text::SetFontPath(const std::string &font) {
    if (entityID == 0) {
        tryGetComponent<TextComponent>().fontPath = font;
    } else
        ComponentStore::GetInstance().tryGetComponent<TextComponent>(entityID).fontPath = font;
}

void Text::SetText(const std::string &text) {
    if (entityID == 0) {
        tryGetComponent<TextComponent>().text = text;
    } else
        ComponentStore::GetInstance().tryGetComponent<TextComponent>(entityID).text = text;
}
