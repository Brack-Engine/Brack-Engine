//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_AUDIOCOMPONENT_HPP
#define BRACK_ENGINE_AUDIOCOMPONENT_HPP

#include "IComponent.hpp"
#include "../../src/Components/ComponentVisitor.hpp"
#include <string>
#include <Components/Archetypes/AudioArchetype.hpp>
#include <utility>

struct AudioComponent : public AudioArchetype {

    explicit AudioComponent(std::string  path, const int channel, bool isSoundTrack) : AudioArchetype(), audioPath(std::move(path)), channel(channel), isSoundTrack(isSoundTrack) {}

    ~AudioComponent() = default;

    void Accept(ComponentVisitor &visitor) override {
        visitor.visit(*this);
    }

    virtual std::unique_ptr<IComponent> clone() const override {
        return std::make_unique<AudioComponent>(*this);
    }

    std::string audioPath;
    bool isLooping = false;
    bool isPlaying = false;
    bool shouldBePlaying = false;
    bool isSoundTrack = false;
    float volume;
    int duration;
    int channel;
};

#endif //BRACK_ENGINE_AUDIOCOMPONENT_HPP
