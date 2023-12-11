//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_SPRITECOMPONENT_HPP
#define BRACK_ENGINE_SPRITECOMPONENT_HPP


#include "Helpers/Color.hpp"
#include "Helpers/Vector2.hpp"
#include <string>
#include <memory>
#include <Components/Archetypes/RenderArchetype.hpp>

struct SpriteComponent : public RenderArchetype {
    explicit SpriteComponent() : RenderArchetype() {}

    ~SpriteComponent() override {
        if (spriteSize != nullptr)
            spriteSize = nullptr;

        if (tileOffset != nullptr)
            tileOffset = nullptr;

        if (color != nullptr)
            color = nullptr;
    };

    std::unique_ptr<IComponent> clone() const override {
        return std::make_unique<SpriteComponent>(*this);
    }

    SpriteComponent(const SpriteComponent &other) : RenderArchetype(other) {
        spritePath = other.spritePath;

        if (other.spriteSize != nullptr)
            spriteSize = std::make_unique<Vector2>(*other.spriteSize);

        if (other.tileOffset != nullptr)
            tileOffset = std::make_unique<Vector2>(*other.tileOffset);

        if (other.color != nullptr)
            color = std::make_unique<Color>(*other.color);

        margin = other.margin;
    }

    std::string spritePath = "";
    std::unique_ptr<Vector2> spriteSize = std::make_unique<Vector2>(0, 0);
    std::unique_ptr<Vector2> tileOffset = std::make_unique<Vector2>(0, 0);
    std::unique_ptr<Color> color = std::make_unique<Color>(0, 0, 0, 255);
    int margin = 0;
};

#endif //BRACK_ENGINE_SPRITECOMPONENT_HPP
