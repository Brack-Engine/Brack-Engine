//
// Created by jesse on 31/10/2023.
//

#include <Components/BehaviourScript.hpp>
#include "BehaviourScriptSystem.hpp"

BehaviourScriptSystem::BehaviourScriptSystem() {

}

BehaviourScriptSystem::~BehaviourScriptSystem() {

}

void BehaviourScriptSystem::Update(float deltaTime) {
    auto entities = ComponentStore::GetInstance().getEntitiesWithComponent<BehaviourScript>();
    for (auto entity: entities) {
        auto& behaviourScriptComponent = ComponentStore::GetInstance().tryGetComponent<BehaviourScript>(entity);
        behaviourScriptComponent.onUpdate(deltaTime);
    }
}

const std::string BehaviourScriptSystem::GetName() const {
    return "BehaviourScriptSystem";
}

void BehaviourScriptSystem::CleanUp() {

}
