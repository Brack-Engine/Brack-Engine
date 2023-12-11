//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_GAMEOBJECT_HPP
#define BRACK_ENGINE_GAMEOBJECT_HPP


#include "Components/IComponent.hpp"
#include <vector>
#include <string>
#include <typeinfo>
#include <stdexcept>
#include <optional>
#include "../src/includes/ComponentStore.hpp"
#include "../Entity.hpp"
#include "../src/includes/BehaviourScriptStore.hpp"

class GameObject {
public:
    GameObject();

    GameObject(entity id);

    ~GameObject() {
        children.clear();
        components.clear();
    };

    GameObject &operator=(const GameObject &other) {
        if (this != &other) {
            entityID = other.entityID;
            components.clear();
            behaviourScripts.clear();
            for (const auto &comp: other.components) {
                components.push_back(comp->clone());
            }
            for (const auto &comp: other.behaviourScripts) {
                behaviourScripts.push_back(comp->clone());
            }
        }
        return *this;
    }

    std::unique_ptr<GameObject> clone() {
        return std::make_unique<GameObject>(*this);
    }

    bool operator==(const GameObject &other) const {
        return (this->entityID == other.entityID); // Assuming 'id' is a unique identifier for GameObjects
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IComponent, T>::value>::type
    addComponent(std::unique_ptr<T> component) {

        if (entityID == 0)
            components.push_back(std::move(component));
        else
            ComponentStore::GetInstance().addComponent<T>(entityID, *component.get());
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IComponent, T>::value>::type
    addComponent(T component) {
        addComponent(std::make_unique<T>(component));
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IBehaviourScript, T>::value>::type
    addBehaviourScript(T component) {
        addBehaviourScript(std::make_unique<T>(component));
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IBehaviourScript, T>::value>::type
    addBehaviourScript(std::unique_ptr<T> component) {

        if (entityID == 0)
            behaviourScripts.push_back(std::move(component));
        else
            BehaviourScriptStore::getInstance().addBehaviourScript<T>(entityID, *component.get());
    }

    GameObject(const GameObject &other) {
        entityID = other.entityID;
        components = std::vector<std::unique_ptr<IComponent>>();
        behaviourScripts = std::vector<std::unique_ptr<IBehaviourScript>>();
        for (auto &comp: other.components) {
            components.push_back(comp->clone());
        }
        for (auto &comp: other.behaviourScripts) {
            behaviourScripts.push_back(comp->clone());
        }
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IComponent, T>::value, bool>::type
    hasComponent() const {
        for (const auto &comp: components) {
            if (dynamic_cast<const T *>(comp.get()) != nullptr) {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IBehaviourScript, T>::value, bool>::type
    hasBehaviourScript() const {
        for (const auto &comp: behaviourScripts) {
            if (dynamic_cast<const T *>(comp.get()) != nullptr) {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IComponent, T>::value, T &>::type
    tryGetComponent() const {
        if (entityID == 0) {
            for (const auto &comp: components) {
                if (auto castedComp = dynamic_cast<T *>(comp.get())) {
                    return *castedComp; // dereference the pointer to return a reference
                }
            }
        } else {
            return ComponentStore::GetInstance().tryGetComponent<T>(entityID);
        }

        throw std::runtime_error("Component not found");
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IComponent, T>::value>::type
    removeComponent() {
        if (entityID == 0) {
            for (auto it = components.begin(); it != components.end();) {
                T *comp = dynamic_cast<T *>(it->get());
                if (comp != nullptr) {
                    it = components.erase(it);
                } else {
                    ++it;
                }
            }
        } else {
            ComponentStore::GetInstance().removeComponent<T>(entityID);
        }
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IBehaviourScript, T>::value>::type
    removeBehaviourScript() {
        if (entityID == 0) {
            for (auto it = behaviourScripts.begin(); it != behaviourScripts.end();) {
                T *comp = dynamic_cast<T *>(it->get());
                if (comp != nullptr) {
                    it = behaviourScripts.erase(it);
                } else {
                    ++it;
                }
            }
        } else {
            BehaviourScriptStore::getInstance().removeBehaviourScript<T>(entityID);
        }
    }

    std::optional<GameObject> getParent();

    std::vector<std::unique_ptr<GameObject>> &&getChildren();

    void addChild(std::unique_ptr<GameObject> child);

    std::string getName() const;

    void setName(std::string name) const;

    std::string getTag() const;

    void setTag(std::string name) const;

    bool isActive() const;

    virtual void setActive(bool active) const;

    int getLayer() const;

    void setRotation(float rotation) const;

    void setLayer(int layer) const;

    entity getEntityId() const;

    void setEntityId(entity id);

    std::vector<std::unique_ptr<IComponent>> &&getAllComponents();

    std::vector<std::unique_ptr<IBehaviourScript>> &&getAllBehaviourScripts();


protected:
    entity entityID = 0;
    std::vector<std::unique_ptr<IComponent>> components;
    std::vector<std::unique_ptr<IBehaviourScript>> behaviourScripts;

    GameObject *parent = nullptr;
    std::vector<std::unique_ptr<GameObject>> children;
};


#endif //BRACK_ENGINE_GAMEOBJECT_HPP
