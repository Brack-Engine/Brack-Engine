//
// Created by jesse on 04/11/2023.
//

#ifndef BRACK_ENGINE_GAMEOBJECTCONVERTER_HPP
#define BRACK_ENGINE_GAMEOBJECTCONVERTER_HPP


#include <cstdint>
#include <optional>
#include "Objects/Camera.hpp"
#include "../outfacingInterfaces/Entity.hpp"

class GameObjectConverter {
public:

    static void addGameObject(std::unique_ptr<GameObject> &gameObject);

    static std::optional<GameObject*> getGameObjectByName(const std::string &name);
    
    static std::optional<GameObject> getGameObjectByTag(const std::string &tag);

    static std::vector<GameObject*> getGameObjectsByTag(const std::string &tag);

    static std::vector<GameObject> getChildren(entity entityID);

    static std::optional<GameObject> getParent(entity entityID);

    static void removeGameObject(std::unique_ptr<GameObject> &gameObject);
};


#endif //BRACK_ENGINE_GAMEOBJECTCONVERTER_HPP
