//
// Created by Stef van Stipdonk on 29/10/2023.
//

#include <unordered_map>
#include "../../includes/SystemManager.hpp"
#include "Objects/Scene.hpp"

SystemManager SystemManager::instance;

void SystemManager::AddSystems(std::vector<std::shared_ptr<ISystem>> newSystems, bool printGraph) {
    for (auto &system: newSystems) {
        Logger::info("Added system " + system->getName());
        systems.push_back(system); // No need to std::move
    }

    if (printGraph)
        PrintDependencyGraph();

    SortSystems();
}

void SystemManager::AddSystem(std::shared_ptr<ISystem> system, bool printGraph) {
    Logger::info("Added system " + system->getName());
    systems.push_back(system);

    if (printGraph)
        PrintDependencyGraph();

    SortSystems();
}

void SystemManager::UpdateSystems(milliseconds deltaTime) {
    for (auto &system: systems) {
        system->update(deltaTime);
    }
}

SystemManager &SystemManager::GetInstance() {
    return instance;
}

void SystemManager::SortSystems() {
    std::vector<std::shared_ptr<ISystem>> sortedList;
    std::vector<ISystem *> nodesWithoutIncomingEdges;
    std::unordered_map<ISystem *, std::vector<std::weak_ptr<ISystem>>> edgeCopy;

    for (auto &system: systems) {
        std::vector<std::weak_ptr<ISystem>> incomingEdges;
        for (auto &weakEdge: system->incomingEdges) {
            if (auto edge = weakEdge.lock()) {
                incomingEdges.push_back(weakEdge);
            }
        }
        edgeCopy[system.get()] = incomingEdges;
        if (incomingEdges.empty()) {
            nodesWithoutIncomingEdges.push_back(system.get());
        }
    }

    while (!nodesWithoutIncomingEdges.empty()) {
        auto node = nodesWithoutIncomingEdges.back();
        nodesWithoutIncomingEdges.pop_back();

        auto it = std::find_if(systems.begin(), systems.end(),
                               [node](const std::shared_ptr<ISystem> &system) { return system.get() == node; });
        if (it != systems.end()) {
            sortedList.push_back(*it); // No need to std::move
            systems.erase(it);
        }

        for (auto &weakDependency: node->outgoingEdges) {
            if (auto dependency = weakDependency.lock()) {
                auto &depIncomingEdgesCopy = edgeCopy[dependency.get()];
                depIncomingEdgesCopy.erase(
                        std::remove_if(depIncomingEdgesCopy.begin(), depIncomingEdgesCopy.end(),
                                       [node](const std::weak_ptr<ISystem> &weakPtr) {
                                           auto ptr = weakPtr.lock();
                                           return ptr.get() == node;
                                       }),
                        depIncomingEdgesCopy.end()
                );

                if (depIncomingEdgesCopy.empty()) {
                    nodesWithoutIncomingEdges.push_back(dependency.get());
                }
            }
        }
    }

    std::string errorString;
    for (auto &pair: edgeCopy) {
        if (!pair.second.empty()) {
            std::string systemName = pair.first->getName();
            int edgeCount = pair.second.size();
            std::string incomingEdgeNames;
            for (auto &weakIncomingEdgeSystem: pair.second) {
                if (auto incomingEdgeSystem = weakIncomingEdgeSystem.lock()) {
                    incomingEdgeNames += incomingEdgeSystem->getName() + " ";
                } else {
                    incomingEdgeNames += "UnknownSystem ";
                }
            }
            errorString += "Graph has a cycle. System '" + systemName + "' has " +
                           std::to_string(edgeCount) + " incoming edge(s) from: " + incomingEdgeNames + "\n";
        }
    }

    if (!errorString.empty()) {
        Logger::error(errorString);
    }

    systems = std::move(sortedList);
}

void SystemManager::PrintDependencyGraph() const {
#if CURRENT_LOG_LEVEL > LOG_LEVEL_ERROR
    std::cout << "===========================\n";
    std::cout << "Dependency Graph:" << std::endl;

    for (const auto &system: systems) {
        std::cout << system->getName() << " depends on: ";
        for (const auto &dependency: system->getDependencies()) {
            if (auto dep = dependency.lock()) {
                std::cout << dep->getName() << ", ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "===========================\n";
#endif
}

void SystemManager::CleanUp() {
    systems.clear();
}

void SystemManager::clearSystems() {
    systems.clear();
}
