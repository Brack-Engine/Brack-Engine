//
// Created by Olaf van den Berg on 07/12/2023.
//


#include "../../includes/BehaviourScriptStore.hpp"

BehaviourScriptStore BehaviourScriptStore::instance;

BehaviourScriptStore &BehaviourScriptStore::getInstance() {
    return instance;
}
