//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_AUDIOSYSTEM_HPP
#define BRACK_ENGINE_AUDIOSYSTEM_HPP


#include <cstdint>
#include <memory>
#include "ISystem.hpp"
#include "../Wrappers/AudioWrapper.hpp"
#include "../outfacingInterfaces/Entity.hpp"

class AudioSystem : public ISystem {
public:
    AudioSystem();

    ~AudioSystem() override;

    const std::string getName() const override;

    void cleanUp() override;

    void clearCache() override;

    void update(milliseconds deltaTime) override;

    AudioSystem(const AudioSystem &other);

    void setAudioWrapper(std::unique_ptr<AudioWrapper> wrapper);

private:
    std::unique_ptr<AudioWrapper> audioWrapper;
};


#endif //BRACK_ENGINE_AUDIOSYSTEM_HPP
