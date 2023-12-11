//
// Created by jesse on 31/10/2023.
//

#include <Components/AnimationComponent.hpp>
#include <Components/SpriteComponent.hpp>
#include "AnimationSystem.hpp"
#include "../includes/ComponentStore.hpp"

AnimationSystem::AnimationSystem() {

}

AnimationSystem::~AnimationSystem() {

}

void AnimationSystem::update(milliseconds deltaTime) {
    auto animationComponentIds = ComponentStore::GetInstance().getEntitiesWithComponent<AnimationComponent>();


    for (auto entityId: animationComponentIds) {

        auto &animationComponent = ComponentStore::GetInstance().tryGetComponent<AnimationComponent>(entityId);
        if (animationComponent.imageSize->getX() == 0 && animationComponent.imageSize->getY() == 0) {
            Logger::GetInstance().Error("Image size is 0,0");
            return;
        }
        if (animationComponent.isPlaying) {
            animationComponent.elapsedTime += deltaTime;
            float frameDuration = 1000.0f / animationComponent.fps;

            auto &spriteComponent = ComponentStore::GetInstance().tryGetComponent<SpriteComponent>(entityId);

            if (animationComponent.elapsedTime >= frameDuration) {
                animationComponent.elapsedTime -= frameDuration;
                animationComponent.currentFrame++;
                if (animationComponent.currentFrame >= animationComponent.frameCount) {
                    if (!animationComponent.isLooping) {
                        animationComponent.isPlaying = false;
                    }
                    spriteComponent.tileOffset = std::make_unique<Vector2>(*animationComponent.startPosition);
                    animationComponent.currentFrame = 0;

                    continue;

                }

                int spriteAmountX = round(animationComponent.imageSize->getX() / spriteComponent.spriteSize->getX());
                int newX = animationComponent.startPosition->getX() + animationComponent.currentFrame;
                int newY = animationComponent.startPosition->getY();

                while (newX >= spriteAmountX) {
                    newX = newX - spriteAmountX;
                    newY++;
                }

                spriteComponent.tileOffset = std::make_unique<Vector2>(newX, newY);
            }
        }
    }
}

const std::string AnimationSystem::getName() const {
    return "AnimationSystem";
}

void AnimationSystem::cleanUp() {
}

void AnimationSystem::clearCache() {

}
