//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_PARENTCOMPONENT_HPP
#define BRACK_ENGINE_PARENTCOMPONENT_HPP

#include "IComponent.hpp"
#include "../../src/Components/ComponentVisitor.hpp"


struct ParentComponent : public IComponent {

    explicit ParentComponent() : IComponent() {}

    ~ParentComponent() override = default;

    virtual std::unique_ptr<IComponent> clone() const override {
        return std::make_unique<ParentComponent>(*this);
    }

    void accept(ComponentVisitor &visitor) override {
        visitor.visit(*this);
    }

    ParentComponent(const ParentComponent &other) : IComponent(other) {
        parentId = other.parentId;
    }

    entity parentId;
};

#endif //BRACK_ENGINE_PARENTCOMPONENT_HPP
