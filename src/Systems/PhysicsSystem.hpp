//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_PHYSICSSYSTEM_HPP
#define BRACK_ENGINE_PHYSICSSYSTEM_HPP


#include <memory>
#include "ISystem.hpp"
#include "../Wrappers/PhysicsWrapper.hpp"

class PhysicsSystem : public ISystem {
public:
    PhysicsSystem();

    ~PhysicsSystem() override;

    void update(milliseconds deltaTime) override;

    const std::string getName() const override;

    void cleanUp() override;

    void clearCache() override;

    PhysicsSystem(const PhysicsSystem &other);

private:
    std::unique_ptr<PhysicsWrapper> physicsWrapper;

    void handleCircles();

    void handleBoxes();

    const milliseconds timeStep = milliseconds(1000.0f / 60.0f);
    milliseconds accumulator = milliseconds(0);
};


#endif //BRACK_ENGINE_PHYSICSSYSTEM_HPP
