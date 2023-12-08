//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_OBJECTINFOCOMPONENT_HPP
#define BRACK_ENGINE_OBJECTINFOCOMPONENT_HPP

#include "IComponent.hpp"
#include <string>

struct ObjectInfoComponent : public IComponent {

    explicit ObjectInfoComponent() : IComponent() {}

    ~ObjectInfoComponent() override = default;

    virtual std::unique_ptr<IComponent> clone() const override {
        return std::make_unique<ObjectInfoComponent>(*this);
    }

    ObjectInfoComponent(const ObjectInfoComponent &other) : IComponent(other) {
        name = other.name;
        tag = other.tag;
        layer = other.layer;
        isActive = other.isActive;
    }

    std::string name, tag = "";
    int layer;
    bool isActive = true;
};

#endif //BRACK_ENGINE_OBJECTINFOCOMPONENT_HPP
