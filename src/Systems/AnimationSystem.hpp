//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_ANIMATIONSYSTEM_HPP
#define BRACK_ENGINE_ANIMATIONSYSTEM_HPP


#include "ISystem.hpp"

class AnimationSystem : public ISystem {
public:
    AnimationSystem();

    ~AnimationSystem() override;

    void Update(float deltaTime) override;
};


#endif //BRACK_ENGINE_ANIMATIONSYSTEM_HPP
