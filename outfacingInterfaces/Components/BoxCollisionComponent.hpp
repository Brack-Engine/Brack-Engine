//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_BOXCOLLISIONCOMPONENT_HPP
#define BRACK_ENGINE_BOXCOLLISIONCOMPONENT_HPP

#include <memory>
#include "../../src/Components/ComponentVisitor.hpp"
#include "CollisionComponent.hpp"

struct BoxCollisionComponent : public CollisionComponent {
    explicit BoxCollisionComponent(float width, float height) : CollisionComponent(), size(new Vector2(width, height)) {}
    explicit BoxCollisionComponent(float size) : BoxCollisionComponent(size,size) {}

    virtual std::unique_ptr<IComponent> clone() const override {
        return std::make_unique<BoxCollisionComponent>(*this);
    }

    ~BoxCollisionComponent() override {
        if(size != nullptr)
            size = nullptr;
    };

    BoxCollisionComponent(const BoxCollisionComponent &other) : CollisionComponent(other) {
        if(other.size != nullptr)
            size = std::make_unique<Vector2>(*other.size);
    }


    void Accept(ComponentVisitor &visitor) override {
        visitor.visit(*this);
    }

    std::unique_ptr<Vector2> size;
};

#endif //BRACK_ENGINE_BOXCOLLISIONCOMPONENT_HPP
