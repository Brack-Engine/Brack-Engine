//
// Created by Olaf van den Berg on 07/12/2023.
//

#ifndef BRACKOCALYPSE_BEHAVIOURSCRIPTSTORE_HPP
#define BRACKOCALYPSE_BEHAVIOURSCRIPTSTORE_HPP


#include <unordered_map>
#include <typeindex>
#include <vector>
#include <memory>
#include <random>
#include "../Logger.hpp"
#include "EntityManager.hpp"
#include "BehaviourScripts/IBehaviourScript.hpp"


class BehaviourScriptStore {
public:
    BehaviourScriptStore(const BehaviourScriptStore &) = delete;

    BehaviourScriptStore &operator=(const BehaviourScriptStore &) = delete;

    BehaviourScriptStore(BehaviourScriptStore &&) = delete;

    BehaviourScriptStore &operator=(BehaviourScriptStore &&) = delete;

    static BehaviourScriptStore &getInstance();

    ~BehaviourScriptStore() = default;

    template<typename T, typename... Args>
    typename std::enable_if<std::is_base_of<IBehaviourScript, T>::value>::type
    addBehaviourScript(Args &&... args) {
        T script(std::forward<Args>(args)...);
        auto entityId = script.entityId;

        if (entityId == 0)
            throw std::runtime_error(
                "Entity ID cannot be 0, please make sure to implement a copy constructor for your component of type " +
                std::string(typeid(T).name()));

        notStartedBehaviourScripts[entityId].emplace_back(std::make_unique<T>(script));
    }

    template<typename T, typename... Args>
    typename std::enable_if<std::is_base_of<IBehaviourScript, T>::value>::type
    addBehaviourScript(entity entityId, Args &&... args) {
        if (entityId == 0)
            throw std::runtime_error(
                "Entity ID cannot be 0, please make sure to implement a copy constructor for your component of type " +
                std::string(typeid(T).name()));

        T script(std::forward<Args>(args)...);
        script.entityId = entityId;

        notStartedBehaviourScripts[entityId].emplace_back(std::make_unique<T>(script));
    }

    void addBehaviourScript(entity entityId, std::unique_ptr<IBehaviourScript> script) {
        if (entityId == 0)
            throw std::runtime_error("Entity ID cannot be 0.");

        script->entityId = entityId;

        notStartedBehaviourScripts[entityId].emplace_back(std::move(script));
    }

    void addActiveBehaviourScript(entity entityId, std::unique_ptr<IBehaviourScript> script) {
        if (entityId == 0)
            throw std::runtime_error("Entity ID cannot be 0.");

        script->entityId = entityId;

        behaviourScripts[entityId].emplace_back(std::move(script));
    }

    void clearBehaviourScripts() {
        behaviourScripts.clear();
    }

    template<typename T>
    std::vector<std::reference_wrapper<T> > getBehaviourScripts(entity entityId) {
        std::vector<std::reference_wrapper<T> > result;

        auto itType = behaviourScripts.find(entityId);
        if (itType != behaviourScripts.end()) {
            for (auto &script: itType->second) {
                if (EntityManager::getInstance().isEntityActive(script.get()->entityId) && script.get()->isActive)
                    result.push_back(std::ref(static_cast<T &>(*script)));
            }
        }
        return result;
    }

    template<typename T>
    std::vector<std::reference_wrapper<T> > getBehaviourScripts() {
        std::vector<std::reference_wrapper<T> > result;

        for (const auto &entry: behaviourScripts) {
            const std::vector<std::unique_ptr<IBehaviourScript> > &scriptVector = entry.second;

            for (const auto &scriptPtr: scriptVector) {
                if (EntityManager::getInstance().isEntityActive(scriptPtr.get()->entityId) && scriptPtr.get()->isActive)
                    result.push_back(std::ref(static_cast<T &>(*scriptPtr)));
            }
        }
        return result;
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IBehaviourScript, T>::value>::type
    removeBehaviourScript(entity entityId) {
        auto itType = behaviourScripts.find(entityId);
        if (itType != behaviourScripts.end()) {
            auto itScript = static_cast<T *>(itType->second);
            if (itScript != nullptr) {
                itScript.erase();
            }
        }
    }

    void removeAllBehaviourScripts(entity entityId) {
        behaviourScripts.erase(entityId);
    }

    std::vector<std::reference_wrapper<IBehaviourScript> > getAbsolutelyAllBehaviourScripts() {
        std::vector<std::reference_wrapper<IBehaviourScript> > result;

        // Iterate through the map
        for (const auto &entry: behaviourScripts) {
            const std::vector<std::unique_ptr<IBehaviourScript> > &scriptVector = entry.second;

            // Iterate through the vector and add references to the result
            for (const auto &scriptPtr: scriptVector) {
                result.push_back(std::ref(*scriptPtr));
            }
        }
        return result;
    }

    std::vector<std::reference_wrapper<IBehaviourScript> > getAllBehaviourScripts() {
        std::vector<std::reference_wrapper<IBehaviourScript> > result;

        // Iterate through the map
        for (const auto &entry: behaviourScripts) {
            const std::vector<std::unique_ptr<IBehaviourScript> > &scriptVector = entry.second;

            // Iterate through the vector and add references to the result
            for (const auto &scriptPtr: scriptVector) {
                if (EntityManager::getInstance().isEntityActive(scriptPtr.get()->entityId) && scriptPtr.get()->isActive)
                    result.push_back(std::ref(*scriptPtr));
            }
        }
        return result;
    }

    std::vector<std::reference_wrapper<IBehaviourScript> > getAllNotStartedBehaviourScripts() {
        std::vector<std::reference_wrapper<IBehaviourScript> > result;

        // Iterate through the map
        for (const auto &entry: notStartedBehaviourScripts) {
            const std::vector<std::unique_ptr<IBehaviourScript> > &scriptVector = entry.second;

            // Iterate through the vector and add references to the result
            for (const auto &scriptPtr: scriptVector) {
                if (EntityManager::getInstance().isEntityActive(scriptPtr.get()->entityId) && scriptPtr.get()->isActive)
                    result.push_back(std::ref(*scriptPtr));
            }
        }
        return result;
    }

    template<typename T>
    typename std::enable_if<std::is_base_of<IBehaviourScript, T>::value, T &>::type
    tryGetBehaviourScript(entity entityId) {
        auto itType = behaviourScripts.find(entityId);
        if (itType != behaviourScripts.end()) {
            for (auto &script: itType->second) {
                if (auto castedScript = static_cast<T *>(script.get())) {
                    return *castedScript;
                }
            }
        }

        itType = notStartedBehaviourScripts.find(entityId);
        if(itType != notStartedBehaviourScripts.end()) {
            for (auto &script: itType->second) {
                if (auto castedScript = static_cast<T *>(script.get())) {
                    return *castedScript;
                }
            }
        }

        throw std::runtime_error("Component not found");
    }

    void moveToStartedScripts(std::reference_wrapper<IBehaviourScript> wrapper) {
        auto entityId = wrapper.get().entityId;
        auto it = notStartedBehaviourScripts.find(entityId);
        if (it != notStartedBehaviourScripts.end()) {
            auto &scripts = it->second;
            auto itScript = std::find_if(scripts.begin(), scripts.end(), [&wrapper](const auto &script) {
                return script.get() == &wrapper.get();
            });
            if (itScript != scripts.end()) {
                behaviourScripts[entityId].emplace_back(std::move(*itScript));
                scripts.erase(itScript);
            }
        }
    }

    void removeBehaviourOfEntity(entity entityId);

private:
    static BehaviourScriptStore instance;

    BehaviourScriptStore() = default;

    std::map<entity, std::vector<std::unique_ptr<IBehaviourScript> > > notStartedBehaviourScripts;

    std::map<entity, std::vector<std::unique_ptr<IBehaviourScript> > > behaviourScripts;
};


#endif //BRACKOCALYPSE_BEHAVIOURSCRIPTSTORE_HPP
