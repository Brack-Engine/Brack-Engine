//
// Created by jesse on 31/10/2023.
//

#ifndef BRACK_ENGINE_PHYSICSWRAPPER_HPP
#define BRACK_ENGINE_PHYSICSWRAPPER_HPP

#include <memory>
#include <vector>
#include <Components/RigidBodyComponent.hpp>
#include "box2d/box2d.h"
#include <unordered_map>

class ContactListener;

class PhysicsWrapper {
public:

    ~PhysicsWrapper();

    static PhysicsWrapper &getInstance();


    PhysicsWrapper(const PhysicsWrapper &other);

    void operator=(const PhysicsWrapper &) = delete;

    PhysicsWrapper(PhysicsWrapper &&other) = delete;

    void operator=(PhysicsWrapper &&) = delete;

    void update(float deltaTime);

    void addCircles(std::vector<uint32_t> componentIds);

    void addBoxes(std::vector<uint32_t> componentIds);

    void cleanCache();

    std::unordered_map<uint32_t, b2Body *> bodies;
    std::vector<uint32_t> boxBodies;
    std::vector<uint32_t> circleBodies;
private:
    PhysicsWrapper();

    static PhysicsWrapper instance;

    std::unique_ptr<ContactListener> contactListener;

    std::unique_ptr<b2World> world;

    b2BodyType getBodyType(CollisionType collisionType);
};

class ContactListener : public b2ContactListener {
public:
    ContactListener() = default;

    void BeginContact(b2Contact *contact) override;

    void EndContact(b2Contact *contact) override;
};


#endif //BRACK_ENGINE_PHYSICSWRAPPER_HPP
