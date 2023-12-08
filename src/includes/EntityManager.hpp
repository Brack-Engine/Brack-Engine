//
// Created by Stef van Stipdonk on 29/10/2023.
//

#ifndef BRACK_ENGINE_ENTITYMANAGER_HPP
#define BRACK_ENGINE_ENTITYMANAGER_HPP


#include <unordered_set>
#include <map>
#include <vector>
#include <cstdint>
#include "../../outfacingInterfaces/Entity.hpp"

class EntityManager {

public:
    static EntityManager &getInstance();

    ~EntityManager() = default;

    EntityManager(const EntityManager &) = delete;

    EntityManager &operator=(const EntityManager &) = delete;

    EntityManager(EntityManager &&) = delete;

    EntityManager &operator=(EntityManager &&) = delete;

    entity createEntity();

    void destroyEntity(entity entityId);

    const std::unordered_set<entity> &getAllEntities() const;

    void clearAllEntities();

    void addEntityWithName(entity entityId, const std::string &name);

    void addEntityWithTag(entity entityId, const std::string &tag);
    
    void addEntitiesByTags(std::map<std::string, std::vector<entity>> entitiesByTag);

    void addEntitiesByName(std::map<std::string, std::vector<entity>> entitiesByName);

    void addEntity(entity entity);

    entity getEntityByName(const std::string &name) const;

    std::vector<entity> getEntitiesByTag(const std::string &tag) const;

    entity getEntityByTag(const std::string &tag) const;

    std::map<std::string, entity> getEntitiesByNameMap() const;

    std::map<std::string, std::vector<entity>> getEntitiesByTagMap() const;

    void setEntitiesByNameMap(const std::map<std::string, entity> &entitiesByName);

    void setEntitiesByTagMap(const std::map<std::string, std::vector<entity>> &entitiesByTag);

    bool isEntityActive(entity entityID) const;

    void setEntityActive(entity entityID, bool active);

private:
    static EntityManager instance;

    EntityManager() = default;

    std::unordered_set<entity> entities;
    entity nextID = 1;
    std::vector<entity> reserveEntities;
    std::map<entity, std::string> entityToName;
    std::map<entity, std::string> entityToTag;
    std::map<std::string, entity> nameToEntity;
    std::map<std::string, std::vector<entity>> tagToEntity;
    std::map<entity, bool> activeEntities;
};


#endif //BRACK_ENGINE_ENTITYMANAGER_HPP
