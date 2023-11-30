//
// Created by jesse on 31/10/2023.
//

#include <Components/AnimationComponent.hpp>
#include "AnimationSystem.hpp"
#include "../includes/ComponentStore.hpp"

AnimationSystem::AnimationSystem() {

}

AnimationSystem::~AnimationSystem() {

}

void AnimationSystem::update(float deltaTime) {
    auto animationComponentIds = ComponentStore::GetInstance().getEntitiesWithComponent<AnimationComponent>();


    for (auto entityId: animationComponentIds) {

        auto &spriteComponent = ComponentStore::GetInstance().tryGetComponent<SpriteComponent>(entityId);
        auto &animationComponent = ComponentStore::GetInstance().tryGetComponent<AnimationComponent>(entityId);
        if (animationComponent.isPlaying) {
            animationComponent.elapsedTime += deltaTime;
            float frameDuration = 1.0f / (animationComponent.fps / animationComponent.frameCount);

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
                int spriteAmountX = round(spriteComponent.imageSize->getX() / spriteComponent.spriteSize->getX());
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

