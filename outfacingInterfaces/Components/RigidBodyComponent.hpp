//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_RIGIDBODYCOMPONENT_HPP
#define BRACK_ENGINE_RIGIDBODYCOMPONENT_HPP

#include "IComponent.hpp"
#include <Helpers/CollisionType.hpp>

struct RigidBodyComponent : public IComponent {
    explicit RigidBodyComponent(CollisionType type) : IComponent(), collisionType(type) {}

    explicit RigidBodyComponent() : IComponent(), collisionType(CollisionType::STATIC) {}

    ~RigidBodyComponent() override = default;

    virtual std::unique_ptr<IComponent> clone() const {
        return std::make_unique<RigidBodyComponent>(*this);
    }

    RigidBodyComponent(const RigidBodyComponent &other) : IComponent(other) {
        mass = other.mass;
        gravityScale = other.gravityScale;
        collisionType = other.collisionType;
    }


    float mass, gravityScale;
    CollisionType collisionType;
};

#endif //BRACK_ENGINE_RIGIDBODYCOMPONENT_HPP
