//
// Created by Stef van Stipdonk on 29/10/2023.
//

#ifndef BRACK_ENGINE_ISYSTEM_HPP
#define BRACK_ENGINE_ISYSTEM_HPP

#include <algorithm>
#include "vector"
#include "../src/Logger.hpp"
#include "../outfacingInterfaces/Milliseconds.hpp"

class ISystem : public std::enable_shared_from_this<ISystem> {
public:
    virtual ~ISystem() = default;

    virtual void init() {}

    virtual void update(milliseconds deltaTime) = 0;

    virtual const std::string getName() const = 0;

    virtual void cleanUp() = 0;

    void addDependency(std::shared_ptr<ISystem> dependency) {
        auto thisPtr = shared_from_this();

        bool dependencyExists = std::any_of(outgoingEdges.begin(), outgoingEdges.end(),
                                            [&dependency](const std::weak_ptr<ISystem> &weakPtr) {
                                                auto sharedPtr = weakPtr.lock();
                                                return sharedPtr == dependency;
                                            }
        );

        if (!dependencyExists) {
            outgoingEdges.push_back(dependency);
            Logger::info("Dependency added for " + this->getName() + ", " + this->getName() + " now depends on " +
                         dependency->getName());

            bool backLinkExists = std::any_of(dependency->incomingEdges.begin(), dependency->incomingEdges.end(),
                                              [thisPtr](const std::weak_ptr<ISystem> &weakPtr) {
                                                  auto sharedPtr = weakPtr.lock();
                                                  return sharedPtr == thisPtr;
                                              }
            );

            if (!backLinkExists) {
                dependency->incomingEdges.push_back(thisPtr);
            }
        }
    }

    void removeDependency(std::shared_ptr<ISystem> dependency) {
        for (auto &outgoingEdge: dependency->outgoingEdges) {
            auto targetSystem = outgoingEdge.lock();
            if (targetSystem) {
                targetSystem->incomingEdges.erase(
                        std::remove_if(targetSystem->incomingEdges.begin(), targetSystem->incomingEdges.end(),
                                       [&dependency](const std::weak_ptr<ISystem> &weakPtr) {
                                           return weakPtr.lock() == dependency;
                                       }),
                        targetSystem->incomingEdges.end()
                );
            }
        }

        for (auto &incomingEdge: dependency->incomingEdges) {
            auto sourceSystem = incomingEdge.lock();
            if (sourceSystem) {
                sourceSystem->outgoingEdges.erase(
                        std::remove_if(sourceSystem->outgoingEdges.begin(), sourceSystem->outgoingEdges.end(),
                                       [&dependency](const std::weak_ptr<ISystem> &weakPtr) {
                                           return weakPtr.lock() == dependency;
                                       }),
                        sourceSystem->outgoingEdges.end()
                );
            }
        }

        Logger::info("Dependency removed for " + dependency->getName());
    }


    void addDependency(std::weak_ptr<ISystem> weakDependency) {
        auto thisPtr = shared_from_this();

        // Lock the weak pointer to get a shared pointer for comparison
        auto dependency = weakDependency.lock();
        if (!dependency) {
            Logger::error("Dependency is expired or invalid.");
            return;
        }

        bool dependencyExists = std::any_of(outgoingEdges.begin(), outgoingEdges.end(),
                                            [&weakDependency](const std::weak_ptr<ISystem> &existingWeakPtr) {
                                                return existingWeakPtr.lock() == weakDependency.lock();
                                            }
        );

        if (!dependencyExists) {
            outgoingEdges.push_back(weakDependency);
            Logger::info("Dependency added for " + this->getName() + ", " + this->getName() + " now depends on " +
                         dependency->getName());

            bool backLinkExists = std::any_of(dependency->incomingEdges.begin(), dependency->incomingEdges.end(),
                                              [thisPtr](const std::weak_ptr<ISystem> &weakPtr) {
                                                  return weakPtr.lock() == thisPtr;
                                              }
            );

            if (!backLinkExists) {
                dependency->incomingEdges.push_back(thisPtr);
            }
        }
    }


    std::vector<std::weak_ptr<ISystem>> outgoingEdges;
    std::vector<std::weak_ptr<ISystem>> incomingEdges;


    const std::vector<std::weak_ptr<ISystem>> &getDependencies() const { return outgoingEdges; }

private:

};

#endif //BRACK_ENGINE_ISYSTEM_HPP
