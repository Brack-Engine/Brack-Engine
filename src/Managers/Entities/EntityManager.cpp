//
// Created by Stef van Stipdonk on 29/10/2023.
//

#include <string>
#include <algorithm>
#include <Components/PersistenceTag.hpp>
#include "../../includes/EntityManager.hpp"
#include "../../includes/ComponentStore.hpp"

EntityManager EntityManager::instance;

entity EntityManager::createEntity() {
    entity id = nextID++;
    entities.insert(id);
    return id;
}

void EntityManager::destroyEntity(entity entityId) {
    auto name = entityToName.find(entityId);
    nameToEntity.erase(name->second);
    auto tag = entityToTag.find(entityId);
    tagToEntity.erase(name->second);
    entityToTag.erase(entityId);
    entityToName.erase(entityId);
    entities.erase(entityId);
    reserveEntities.push_back(entityId);
}

void EntityManager::addEntitiesByTags(std::map<std::string, std::vector<entity>> entitiesByTag) {
    for (auto &pair: entitiesByTag) {
        for (auto &entity: pair.second) {
            addEntityWithTag(entity, pair.first);
        }
    }
}

void EntityManager::addEntitiesByName(std::map<std::string, std::vector<entity>> entitiesByName) {
    for (auto &pair: entitiesByName) {
        for (auto &entity: pair.second) {
            addEntityWithName(entity, pair.first);
        }
    }
}

const std::unordered_set<entity> &EntityManager::getAllEntities() const {
    return entities;
}

void EntityManager::clearAllEntities() {
    auto persistanceEntities = ComponentStore::GetInstance().getEntitiesWithComponent<PersistenceTag>();

    std::unordered_set<entity> copyEnt(entities);
    for (auto entity: copyEnt) {
        auto found = std::find(persistanceEntities.begin(), persistanceEntities.end(), entity);
        if (found == persistanceEntities.end()) {
            ComponentStore::GetInstance().removeAllComponents(entity);
            entities.erase(entity);
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
        Logger::GetInstance().Warning("Entity with name " + name + " already exists, and will be overwritten.");

    nameToEntity[name] = {entityId};

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

entity EntityManager::getEntityByName(const std::string &name) const {
    if (nameToEntity.find(name) != nameToEntity.end())
        return nameToEntity.find(name)->second;

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

std::map<std::string, entity> EntityManager::getEntitiesByNameMap() const {
    return nameToEntity;
}

std::map<std::string, std::vector<entity>> EntityManager::getEntitiesByTagMap() const {
    return tagToEntity;
}

void EntityManager::setEntitiesByNameMap(const std::map<std::string, entity> &entitiesByName) {
    EntityManager::nameToEntity = entitiesByName;
}

void EntityManager::setEntitiesByTagMap(const std::map<std::string, std::vector<entity>> &entitiesByTag) {
    EntityManager::tagToEntity = entitiesByTag;
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
