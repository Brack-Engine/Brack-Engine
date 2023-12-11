//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_ANIMATIONCOMPONENT_HPP
#define BRACK_ENGINE_ANIMATIONCOMPONENT_HPP

#include <memory>
#include <Helpers/Vector2.hpp>
#include "IComponent.hpp"

struct AnimationComponent : public IComponent {

    explicit AnimationComponent() : IComponent() {
    }

    ~AnimationComponent() override {
        if (startPosition != nullptr)
            startPosition = nullptr;

        if (imageSize != nullptr)
            imageSize = nullptr;
    };

    virtual std::unique_ptr<IComponent> clone() const override {
        return std::make_unique<AnimationComponent>(*this);
    }

    AnimationComponent(const AnimationComponent &other) : IComponent(other) {
        isLooping = other.isLooping;
        isPlaying = other.isPlaying;
        fps = other.fps;
        frameCount = other.frameCount;
        currentFrame = other.currentFrame;
        elapsedTime = other.elapsedTime;
        if (other.startPosition != nullptr)
            startPosition = std::make_unique<Vector2>(*other.startPosition);
        if (other.imageSize != nullptr)
            imageSize = std::make_unique<Vector2>(*other.imageSize);
    }

    bool isLooping = true, isPlaying = true;
    float fps = 0.0f;
    float elapsedTime = 0.0f;
    int frameCount = 0, currentFrame = 0;
    std::unique_ptr<Vector2> startPosition = std::make_unique<Vector2>(0, 0);
    std::unique_ptr<Vector2> imageSize = std::make_unique<Vector2>(0, 0);
};

#endif //BRACK_ENGINE_ANIMATIONCOMPONENT_HPP
