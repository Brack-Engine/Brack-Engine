//
// Created by jesse on 14/11/2023.
//

#ifndef BRACKOCALYPSE_RENDERARCHETYPE_HPP
#define BRACKOCALYPSE_RENDERARCHETYPE_HPP

#include <Components/IComponent.hpp>
#include "../../../src/Components/ComponentVisitor.hpp"


struct RenderArchetype : public IComponent {
public:
    explicit RenderArchetype() : IComponent() {}

    ~RenderArchetype() override = default;

    virtual std::unique_ptr<IComponent> clone() const override {
        return std::make_unique<RenderArchetype>(*this);
    }

    void accept(ComponentVisitor &visitor) override {
        visitor.visit<RenderArchetype>(*this);
    }

    RenderArchetype(const RenderArchetype &other) : IComponent(other) {
        sortingLayer = other.sortingLayer;
        orderInLayer = other.orderInLayer;
        flipX = other.flipX;
        flipY = other.flipY;
    }

    int sortingLayer = 1;
    int orderInLayer = 0;
    bool flipX = false;
    bool flipY = false;
};

#endif //BRACKOCALYPSE_RENDERARCHETYPE_HPP
