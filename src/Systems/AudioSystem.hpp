//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_AUDIOSYSTEM_HPP
#define BRACK_ENGINE_AUDIOSYSTEM_HPP


#include <cstdint>
#include "ISystem.hpp"

class AudioSystem : public ISystem {
public:
    AudioSystem();

    ~AudioSystem() override;

    void Update(float deltaTime) override;

private:
    void PlayPause(uint32_t entityID);
};


#endif //BRACK_ENGINE_AUDIOSYSTEM_HPP
