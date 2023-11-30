//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_BEHAVIOURSCRIPT_HPP
#define BRACK_ENGINE_BEHAVIOURSCRIPT_HPP

#include "IComponent.hpp"
#include "../../src/Components/ComponentVisitor.hpp"
#include "../outfacingInterfaces/Milliseconds.hpp"
#include "../../src/GameObjectConverter.hpp"
#include <functional>
#include <Objects/GameObject.hpp>
#include <optional>

struct BehaviourScript : public IComponent {
    explicit BehaviourScript() = default;

    ~BehaviourScript() override = default;

    virtual std::unique_ptr<IComponent> clone() const override {
        return std::make_unique<BehaviourScript>(*this);
    }

    virtual void accept(ComponentVisitor &visitor) override {
        visitor.visit(*this);
    }

    BehaviourScript(const BehaviourScript &other) : IComponent(other) {}

    virtual void onStart() {};

    virtual void onUpdate(milliseconds deltaTime) {};

    template<typename T>
    typename std::enable_if<std::is_base_of<IComponent, T>::value, T &>::type
    tryGetComponent() {
        return ComponentStore::GetInstance().tryGetComponent<T>(entityID);
    }

    static std::optional<GameObject> getGameObjectByName(const std::string &name) {
        return GameObjectConverter::getGameObjectByName(name);
    }

    static std::vector<GameObject> getGameObjectsByName(const std::string &name) {
        return GameObjectConverter::getGameObjectsByName(name);
    }

    static std::optional<GameObject> getGameObjectByTag(const std::string &tag) {
        return GameObjectConverter::getGameObjectByTag(tag);
    }

    static std::vector<GameObject> getGameObjectsByTag(const std::string &tag) {
        return GameObjectConverter::getGameObjectsByTag(tag);
    }

    std::vector<GameObject> getChildren() {
        return GameObjectConverter::getChildren(entityID);
    }
};


#endif //BRACK_ENGINE_BEHAVIOURSCRIPT_HPP
