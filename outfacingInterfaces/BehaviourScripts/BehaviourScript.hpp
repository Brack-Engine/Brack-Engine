//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_BEHAVIOURSCRIPT_HPP
#define BRACK_ENGINE_BEHAVIOURSCRIPT_HPP

#include "../../src/includes/ComponentStore.hpp"
#include "IBehaviourScript.hpp"
#include <functional>
#include <optional>
#include <Objects/GameObject.hpp>


class BehaviourScript : public IBehaviourScript {
public:

    explicit BehaviourScript() = default;


    BehaviourScript(std::remove_reference<struct std::unique_ptr<struct BehaviourScript> &>::type type) {
        entityId = type->entityId;
        isActive = type->isActive;
    }

    virtual ~BehaviourScript() = default;

    BehaviourScript(const BehaviourScript &other) {
        entityId = other.entityId;
        isActive = other.isActive;
    }

    std::unique_ptr<IBehaviourScript> clone() const = 0;

    virtual void onUpdate(milliseconds deltaTime) {};

    template<typename T>
    typename std::enable_if<std::is_base_of<IComponent, T>::value, T &>::type
    tryGetComponent() {
        return ComponentStore::GetInstance().tryGetComponent<T>(entityId);
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IBehaviourScript, T>::value, T &>::type
    tryGetBehaviourScript() const {
        return BehaviourScriptStore::getInstance().tryGetBehaviourScript<T>(entityId);
    }

    void setActive(bool active) {
        EntityManager::getInstance().setEntityActive(entityId, active);
        tryGetComponent<ObjectInfoComponent>().isActive = active;
    }

    bool getActive() {
        return EntityManager::getInstance().isEntityActive(entityId) && tryGetComponent<ObjectInfoComponent>().isActive;
    }

    static std::optional<GameObject *> getGameObjectByName(const std::string &name);

    static std::vector<GameObject> getGameObjectsByName(const std::string &name);

    static std::optional<GameObject> getGameObjectByTag(const std::string &tag);

    static std::vector<GameObject *> getGameObjectsByTag(const std::string &tag);

    std::vector<GameObject> getChildren();

    std::optional<GameObject> getParent();

};


#endif //BRACK_ENGINE_BEHAVIOURSCRIPT_HPP
