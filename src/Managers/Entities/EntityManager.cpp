//
// Created by Stef van Stipdonk on 29/10/2023.
//

#include <string>
#include <algorithm>
#include <Components/PersistenceTag.hpp>
#include "../../includes/EntityManager.hpp"
#include "../../includes/ComponentStore.hpp"
#include "../../includes/BehaviourScriptStore.hpp"

EntityManager EntityManager::instance;

entity EntityManager::createEntity() {
    entity id = nextID++;
    entities.insert(id);
    return id;
}

void EntityManager::destroyEntity(entity entityId) {
    ComponentStore::GetInstance().removeComponentsOfEntity(entityId);
    auto name = entityToName.find(entityId);
    nameToEntity.erase(name->second);
    auto tag = entityToTag.find(entityId);
    tagToEntity.erase(name->second);
    entityToTag.erase(entityId);
    entityToName.erase(entityId);
    entities.erase(entityId);
    reserveEntities.push_back(entityId);
}

void EntityManager::addEntitiesByTags(std::map<std::string, std::vector<entity> > entitiesByTag) {
    for (auto &pair: entitiesByTag) {
        for (auto &entity: pair.second) {
            addEntityWithTag(entity, pair.first);
        }
    }
}

void EntityManager::addEntitiesByName(std::map<std::string, std::vector<entity> > entitiesByName) {
    for (auto &pair: entitiesByName) {
        for (auto &entity: pair.second) {
            addEntityWithName(entity, pair.first);
        }
    }
}

const std::unordered_set<entity> &EntityManager::getAllEntities() const {
    return entities;
}

const std::map<entity, bool> &EntityManager::getStatesForAllEntities() const {
    return activeEntities;
}

void EntityManager::clearAllEntities() {
    auto persistanceEntities = ComponentStore::GetInstance().getAllEntitiesWithComponent<PersistenceTag>();

    std::unordered_set<entity> copyEnt(entities);
    for (auto entity: copyEnt) {
        auto found = std::find(persistanceEntities.begin(), persistanceEntities.end(), entity);
        if (found == persistanceEntities.end()) {
            ComponentStore::GetInstance().removeAllComponents(entity);
            BehaviourScriptStore::getInstance().removeAllBehaviourScripts(entity);
            entities.erase(entity);
            entityToName.erase(entity);
            entityToTag.erase(entity);

            auto tagToEntityCopy = tagToEntity;
            for (auto &pair: tagToEntityCopy) {
                auto &idVector = tagToEntity[pair.first];
                idVector.erase(std::remove(idVector.begin(), idVector.end(), entity), idVector.end());
                if (idVector.empty()) {
                    tagToEntity.erase(pair.first);
                }
            }

            auto nameToEntityCopy = nameToEntity;
            for (auto &pair: nameToEntityCopy) {
                auto &idVector = nameToEntity[pair.first];
                idVector.erase(std::remove(idVector.begin(), idVector.end(), entity), idVector.end());
                if (idVector.empty()) {
                    nameToEntity.erase(pair.first);
                }
            }
        }
    }
}


EntityManager &EntityManager::getInstance() {
    return instance;
}

void EntityManager::addEntityWithName(entity entityId, const std::string &name) {
    if (name.empty())
        return;
    if (nameToEntity.find(name) == nameToEntity.end())
        nameToEntity[name] = {entityId};
    else if (std::find(nameToEntity[name].begin(), nameToEntity[name].end(), entityId) ==
             nameToEntity[name].end())
        nameToEntity[name].push_back(entityId);

    entityToName[entityId] = name;
}

void EntityManager::addEntityWithTag(entity entityId, const std::string &tag) {
    if (tag.empty())
        return;
    if (tagToEntity.find(tag) == tagToEntity.end())
        tagToEntity[tag] = {entityId};
    else if (std::find(tagToEntity[tag].begin(), tagToEntity[tag].end(), entityId) == tagToEntity[tag].end())
        tagToEntity[tag].push_back(entityId);

    entityToTag[entityId] = tag;
}

void EntityManager::addEntity(entity entity) {
    entities.insert(entity);
}


std::vector<entity> EntityManager::getEntitiesByName(const std::string &name) const {
    if (nameToEntity.find(name) != nameToEntity.end())
        return nameToEntity.at(name);
    return {};
}

entity EntityManager::getEntityByName(const std::string &name) const {
    if (nameToEntity.find(name) != nameToEntity.end())
        return nameToEntity.at(name)[0];
    return 0;
}

std::vector<entity> EntityManager::getEntitiesByTag(const std::string &tag) const {
    if (tagToEntity.find(tag) != tagToEntity.end())
        return tagToEntity.at(tag);
    return {};
}

entity EntityManager::getEntityByTag(const std::string &tag) const {
    if (tagToEntity.find(tag) != tagToEntity.end())
        return tagToEntity.at(tag)[0];
    return 0;
}

std::map<std::string, std::vector<entity> > EntityManager::getEntitiesByNameMap() const {
    return nameToEntity;
}

std::map<std::string, std::vector<entity> > EntityManager::getEntitiesByTagMap() const {
    return tagToEntity;
}

void EntityManager::setEntitiesByNameMap(const std::map<std::string, std::vector<entity> > &entitiesByName) {
    nameToEntity = entitiesByName;
}

void EntityManager::setEntitiesByTagMap(const std::map<std::string, std::vector<entity> > &entitiesByTag) {
    tagToEntity = entitiesByTag;
}

void EntityManager::setActiveEntities(const std::map<entity, bool> &activeEntities) {
    EntityManager::activeEntities = activeEntities;
}


bool EntityManager::isEntityActive(entity entityID) const {
    auto it = activeEntities.find(entityID);
    if (it != activeEntities.end())
        return activeEntities.at(entityID);
    return false;
}

void EntityManager::setEntityActive(entity entityID, bool active) {
    activeEntities[entityID] = active;
}

bool EntityManager::entityExistsByTag(const std::string string) {
    return tagToEntity.find(string) != tagToEntity.end();
}
