//
// Created by jesse on 31/10/2023.
//

#include "AudioSystem.hpp"
#include "../includes/ComponentStore.hpp"

AudioSystem::AudioSystem() : audioWrapper(std::make_unique<AudioWrapper>()) {
}

AudioSystem::~AudioSystem() {
}

void AudioSystem::update(milliseconds deltaTime) {
    auto audiocomponents = ComponentStore::GetInstance().getAllComponentsOfType<AudioArchetype>();
    for (auto audioComponent: audiocomponents) {
        if (audioComponent->startPlaying && !audioComponent->pauseSound) {
            audioComponent->startPlaying = false;
            audioWrapper->playSound(*audioComponent);
        }

        if (audioComponent->pauseSound && audioComponent->startPlaying) {
            audioComponent->pauseSound = false;
            audioComponent->startPlaying = false;
            audioWrapper->resumeSound(*audioComponent);
        } else if (audioComponent->pauseSound) {
            audioWrapper->pauseSound(*audioComponent);
        }
    }
}

const std::string AudioSystem::getName() const {
    return "AudioSystem";
}

void AudioSystem::cleanUp() {
    auto entities = ComponentStore::GetInstance().getEntitiesWithComponent<AudioArchetype>();
    for (auto entity: entities) {
        auto audioComponent = ComponentStore::GetInstance().tryGetComponent<AudioArchetype>(entity);
        audioComponent.startPlaying = false;
    }
    audioWrapper->cleanUp();
}

AudioSystem::AudioSystem(const AudioSystem &other) {
    audioWrapper = std::make_unique<AudioWrapper>(*other.audioWrapper);
}

void AudioSystem::clearCache() {
    audioWrapper->clearChannels();
}
