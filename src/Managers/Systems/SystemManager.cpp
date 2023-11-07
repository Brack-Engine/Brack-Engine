//
// Created by Stef van Stipdonk on 29/10/2023.
//

#include <stdexcept>
#include "../../includes/SystemManager.hpp"
#include "Objects/Scene.hpp"
#include "../../Logger.hpp"
#include "../../FPSSingleton.hpp"

SystemManager SystemManager::instance;

void SystemManager::AddSystem(ISystem *system) {
    systems.push_back(system);
    SortSystems();
}

void SystemManager::UpdateSystems(float deltaTime) {
    for (auto system: systems) {
        system->Update(deltaTime);
    }
}

SystemManager &SystemManager::GetInstance() {
    return instance;
}

void SystemManager::SortSystems() {
    auto IsDependent = [](ISystem *a, ISystem *b) {
        const auto &dependencies = a->GetDependencies();
        return std::find(dependencies.begin(), dependencies.end(), b) != dependencies.end();
    };

    // Perform the topological sort
    std::vector<ISystem *> sorted;
    std::vector<ISystem *> temp(systems);

    while (!temp.empty()) {
        bool acyclic = false;

        for (auto it = temp.begin(); it != temp.end();) {
            auto sys = *it;
            if (std::none_of(temp.begin(), temp.end(), [sys, &IsDependent](ISystem *other) {
                return IsDependent(sys, other);
            })) {
                sorted.push_back(sys);
                it = temp.erase(it);
                acyclic = true;
            } else {
                ++it;
            }
        }

        if (!acyclic) {
            throw std::runtime_error("Cyclic dependency detected");
        }
    }

    systems = sorted;
}

void SystemManager::CleanUp() {
    std::cout << "Finishing systems" << std::endl;
    for (auto system: systems) {
        system->CleanUp();
    }
}
