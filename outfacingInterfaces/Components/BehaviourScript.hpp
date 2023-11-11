//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_BEHAVIOURSCRIPT_HPP
#define BRACK_ENGINE_BEHAVIOURSCRIPT_HPP

#include "IComponent.hpp"
#include "../../src/Components/ComponentVisitor.hpp"
#include <functional>

struct BehaviourScript : public IComponent {
    explicit BehaviourScript() : IComponent() {}

    ~BehaviourScript() override = default;

    void Accept(ComponentVisitor &visitor) override {
        visitor.visit<BehaviourScript>(this);
        OnStart();
    }

    virtual void OnStart() = 0;

    virtual void OnUpdate(float deltaTime) = 0;
};


#endif //BRACK_ENGINE_BEHAVIOURSCRIPT_HPP
