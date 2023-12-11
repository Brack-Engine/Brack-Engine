//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_BUTTON_HPP
#define BRACK_ENGINE_BUTTON_HPP


#include <Helpers/Color.hpp>
#include <functional>
#include "GameObject.hpp"

class Button : public GameObject {
public:
    Button(const Vector2 &size, const std::string &text);

    ~Button() = default;

    void setText(const std::string &string);

    void setTextColor(const Color &color);

    void setFill(const Color &color);

    void setFontPath(const std::string &path);

    void setFontSize(int fontSize);

    void setClickEvent(std::function<void()> func);

    bool isDisabled() const;

    void setDisabled(bool disabled);

};

#endif //BRACK_ENGINE_BUTTON_HPP