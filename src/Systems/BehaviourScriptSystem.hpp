//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_BEHAVIOURSCRIPTSYSTEM_HPP
#define BRACK_ENGINE_BEHAVIOURSCRIPTSYSTEM_HPP


#include "ISystem.hpp"

class BehaviourScriptSystem : public ISystem {
public:
    BehaviourScriptSystem();

    ~BehaviourScriptSystem() override;

    const std::string GetName() const override;

    void CleanUp() override;

    void Update(float deltaTime) override;

private:

};


#endif //BRACK_ENGINE_BEHAVIOURSCRIPTSYSTEM_HPP
