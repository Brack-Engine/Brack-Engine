//
// Created by jesse on 31/10/2023.
//

#include "AudioSystem.hpp"
#include "../includes/EntityManager.hpp"
#include <fstream>
#include <Components/AudioComponent.hpp>

AudioSystem::AudioSystem() : audioWrapper(new AudioWrapper()) {
}

AudioSystem::~AudioSystem() {
}

void AudioSystem::Update(float deltaTime) {
    auto entities = ComponentStore::GetInstance().getEntitiesWithComponent<AudioComponent>();
    for (auto entity : entities) {
        const auto& audioComponents = ComponentStore::GetInstance().GetComponents<AudioComponent>(entity);
        for (const auto& audioComponent : audioComponents) {
            if(!audioWrapper->IsInitialized(*audioComponent)){
                if(audioWrapper->IsValidAudioPath(*audioComponent)){
                    audioWrapper->UploadSound(*audioComponent);
                }
            }

            if(audioComponent->shouldBePlaying){
                audioWrapper->PlaySound(*audioComponent);
            }

            if (audioWrapper->GetVolume(*audioComponent) != audioComponent->volume) {
                audioWrapper->SetVolume(*audioComponent, audioComponent->volume);
            }
            if (audioWrapper->GetLooping(*audioComponent) != audioComponent->isLooping) {
                audioWrapper->SetLooping(*audioComponent, audioComponent->isLooping);
            }
            if(!audioComponent->isLooping){
                if(audioWrapper->HasSoundFinished(*audioComponent)){
                    audioComponent->isPlaying = false;
                    audioComponent->shouldBePlaying = false;
                }
            }
        }
    }
}

const std::string AudioSystem::GetName() const {
    return "AudioSystem";
}

void AudioSystem::CleanUp() {
    auto entities = ComponentStore::GetInstance().getEntitiesWithComponent<AudioComponent>();

    for (auto entity : entities) {
        auto audioComponents = ComponentStore::GetInstance().GetComponents<AudioComponent>(entity);
        for (auto audioComponent : audioComponents) {
            audioWrapper->RemoveSound(*audioComponent);
            audioComponent->isPlaying = false;
        }
    }
    audioWrapper->CleanUp();
}
