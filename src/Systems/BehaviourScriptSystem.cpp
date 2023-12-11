//
// Created by jesse on 31/10/2023.
//

#include <BehaviourScripts/BehaviourScript.hpp>
#include <Components/Archetypes/AudioArchetype.hpp>
#include "BehaviourScriptSystem.hpp"
#include "../includes/BehaviourScriptStore.hpp"

BehaviourScriptSystem::BehaviourScriptSystem() {

}

BehaviourScriptSystem::~BehaviourScriptSystem() {

}


void BehaviourScriptSystem::update(milliseconds deltaTime) {
    auto behaviourScripts = BehaviourScriptStore::getInstance().getAllBehaviourScripts();
    for (auto script: behaviourScripts) {
        script.get().onUpdate(deltaTime);
    }
}


const std::string BehaviourScriptSystem::getName() const {
    return "BehaviourScriptSystem";
}

void BehaviourScriptSystem::cleanUp() {

}

void BehaviourScriptSystem::clearCache() {

}
