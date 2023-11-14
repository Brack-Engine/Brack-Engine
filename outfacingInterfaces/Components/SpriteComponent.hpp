//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_SPRITECOMPONENT_HPP
#define BRACK_ENGINE_SPRITECOMPONENT_HPP

#include "TransformComponent.hpp"
#include "Helpers/Color.hpp"
#include "../../src/Components/ComponentVisitor.hpp"
#include <string>
#include <memory>

struct SpriteComponent : public TransformComponent {
    explicit SpriteComponent() : TransformComponent() {}

    ~SpriteComponent() override = default;

    SpriteComponent(const SpriteComponent &other) : TransformComponent(other) {
        spritePath = other.spritePath;
        imageSize = std::make_unique<Vector2>(*other.imageSize);
        spriteSize = std::make_unique<Vector2>(*other.spriteSize);
        tileOffset = std::make_unique<Vector2>(*other.tileOffset);
        color = std::make_unique<Color>(*other.color);
        flipX = other.flipX;
        flipY = other.flipY;
        sortingLayer = other.sortingLayer;
        orderInLayer = other.orderInLayer;
        margin = other.margin;
    }

    void Accept(ComponentVisitor &visitor) override {
        visitor.visit(*this);
    }

    std::string spritePath;
    std::unique_ptr<Vector2> imageSize, spriteSize, tileOffset;
    std::unique_ptr<Color> color;
    bool flipX, flipY;
    int sortingLayer, orderInLayer;
    int margin;
};

#endif //BRACK_ENGINE_SPRITECOMPONENT_HPP
