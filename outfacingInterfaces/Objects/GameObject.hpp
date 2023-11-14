//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_GAMEOBJECT_HPP
#define BRACK_ENGINE_GAMEOBJECT_HPP


#include "Components/IComponent.hpp"
#include <vector>
#include <string>
#include <typeinfo>

class GameObject {
public:
    GameObject();

    ~GameObject() = default;

    bool operator==(const GameObject &other) const {
        return (this->entityID == other.entityID); // Assuming 'id' is a unique identifier for GameObjects
    }

    template<typename T>
    void AddComponent(T *component) {
        components.push_back(component);
    }

    template<typename T>
    bool HasComponent(T &component) {
        for (auto &comp: components) {
            if (dynamic_cast<T>(comp))
                return true;
        }
        return false;
    }

    template<typename T>
    T *GetComponent() const {
        for (auto &comp: components) {
            if (T *castedComp = dynamic_cast<T *>(comp)) {
                return castedComp;
            }
        }
        return nullptr;
    }

    template<typename T>
    void RemoveComponent() {
        for (auto it = components.begin(); it != components.end();) {
            T *comp = dynamic_cast<T *>(*it);
            if (comp != nullptr) {
                components.erase(it);
                break;
            } else {

                ++it;
            }
        }
    }


    GameObject &GetParent();

    std::vector<GameObject> GetChildren();

    std::string GetName() const;

    void SetName(char *name);

    std::string GetTag() const;

    void SetTag(char *tag);

    bool IsActive() const;

    void SetActive(bool active);

    int GetLayer() const;

    void SetLayer(int layer);

    uint32_t GetEntityID() const;

    void SetEntityID(uint32_t id);

    std::vector<IComponent *> &GetAllComponents();

protected:
    uint32_t entityID = 0;
    std::vector<IComponent *> components;
};


#endif //BRACK_ENGINE_GAMEOBJECT_HPP
