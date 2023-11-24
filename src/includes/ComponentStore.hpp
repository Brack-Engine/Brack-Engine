#ifndef SIMPLE_COMPONENTSTORE_HPP
#define SIMPLE_COMPONENTSTORE_HPP

#include <unordered_map>
#include <typeindex>
#include <vector>
#include <memory>
#include <random>
#include "Components/IComponent.hpp"
#include "../Logger.hpp"
#include "EntityManager.hpp"

class ComponentStore {
public:
    ComponentStore(const ComponentStore &) = delete;

    ComponentStore &operator=(const ComponentStore &) = delete;

    ComponentStore(ComponentStore &&) = delete;

    ComponentStore &operator=(ComponentStore &&) = delete;

    static ComponentStore &GetInstance();

    ~ComponentStore() = default;

    template<typename T, typename...Args>
    typename std::enable_if<std::is_base_of<IComponent, T>::value>::type
    addComponent(Args &&...args) {
        T component(std::forward<Args>(args)...);
        auto entityId = component.entityID;

        if (entityId == 0)
            throw std::runtime_error(
                    "Entity ID cannot be 0, please make sure to implement a copy constructor for your component of type " +
                    std::string(typeid(T).name()));

        components[typeid(T)][entityId] = std::make_unique<T>(component);
    }

    template<typename T, typename...Args>
    typename std::enable_if<std::is_base_of<IComponent, T>::value>::type
    addComponent(entity entityId, Args &&...args) {
        if (entityId == 0)
            throw std::runtime_error(
                    "Entity ID cannot be 0, please make sure to implement a copy constructor for your component of type " +
                    std::string(typeid(T).name()));

        T component(std::forward<Args>(args)...);
        component.entityID = entityId;

        components[typeid(T)][entityId] = std::make_unique<T>(component);
    }


    template<typename T>
    typename std::enable_if<std::is_base_of<IComponent, T>::value, T &>::type
    tryGetComponent(entity entityId) {
        auto itType = components.find(typeid(T));
        if (itType != components.end()) {
            auto itEntity = itType->second.find(entityId);
            if (itEntity != itType->second.end()) {
                // Cast and return a reference to the managed object
                return *static_cast<T *>(itEntity->second.get());
            }
        }
        throw std::runtime_error("Component not found");
    }

    template<typename BaseT>
    typename std::enable_if<std::is_base_of<IComponent, BaseT>::value, std::vector<BaseT *>>::type
    getAllComponentsOfType() {
        std::vector<BaseT *> result;
        for (auto &[type, map]: components) {
            for (auto &[id, comp]: map) {
                BaseT *casted = dynamic_cast<BaseT *>(comp.get());
                if (casted) {
                    result.push_back(casted);
                }
            }
        }
        return result;
    }

    const std::unordered_map<std::type_index, std::unordered_map<entity, std::unique_ptr<IComponent>>> &
    getComponents() const {
        return components;
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IComponent, T>::value>::type
    removeComponent(entity entityId) {
        auto itType = components.find(typeid(T));
        if (itType != components.end()) {
            itType->second.erase(entityId);
        }
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IComponent, T>::value, std::vector<entity>>::type
    getEntitiesWithComponent() {
        std::vector<entity> entities;
        auto itType = components.find(typeid(T));
        if (itType != components.end()) {
            for (auto &pair: itType->second) {
                entities.push_back(pair.first);
            }
        }
        return entities;
    }


private:
    static ComponentStore instance;

    ComponentStore() = default;

    std::unordered_map<std::type_index, std::unordered_map<entity, std::unique_ptr<IComponent>>> components;
};

#endif // SIMPLE_COMPONENTSTORE_HPP