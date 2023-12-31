//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_VELOCITYCOMPONENT_HPP
#define BRACK_ENGINE_VELOCITYCOMPONENT_HPP

#include "IComponent.hpp"
#include "Helpers/Vector2.hpp"

struct VelocityComponent : public IComponent {
    explicit VelocityComponent() : IComponent() {}

    ~VelocityComponent() = default;

    virtual std::unique_ptr<IComponent> clone() const override {
        return std::make_unique<VelocityComponent>(*this);
    }

    VelocityComponent(const VelocityComponent &other) : IComponent(other) {
        velocity = other.velocity;
    }

    Vector2 velocity;
};

#endif //BRACK_ENGINE_VELOCITYCOMPONENT_HPP
