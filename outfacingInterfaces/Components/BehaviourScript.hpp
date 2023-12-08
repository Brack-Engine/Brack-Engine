//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_BEHAVIOURSCRIPT_HPP
#define BRACK_ENGINE_BEHAVIOURSCRIPT_HPP

#include "IComponent.hpp"
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
    
    static std::optional<GameObject> getGameObjectByTag(const std::string &tag) {
        return GameObjectConverter::getGameObjectByTag(tag);
    }

    static std::vector<GameObject> getGameObjectsByTag(const std::string &tag) {
        return GameObjectConverter::getGameObjectsByTag(tag);
    }

    std::vector<GameObject> getChildren() {
        return GameObjectConverter::getChildren(entityID);
    }

    std::optional<GameObject> getParent() {
        return GameObjectConverter::getParent(entityID);
    }

    void setActive(bool active) {
        EntityManager::getInstance().setEntityActive(entityID, active);
        tryGetComponent<ObjectInfoComponent>().isActive = active;
    }
};


#endif //BRACK_ENGINE_BEHAVIOURSCRIPT_HPP
