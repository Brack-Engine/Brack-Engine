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
#include <Milliseconds.hpp>
#include <Helpers/Vector2.hpp>
#include <Components/BoxCollisionComponent.hpp>
#include <Components/CircleCollisionComponent.hpp>

class ContactListener;

class PhysicsWrapper {
public:

    ~PhysicsWrapper();

    static PhysicsWrapper &getInstance();


    PhysicsWrapper(const PhysicsWrapper &other);

    void operator=(const PhysicsWrapper &) = delete;

    PhysicsWrapper(PhysicsWrapper &&other) = delete;

    void operator=(PhysicsWrapper &&) = delete;

    void update(milliseconds deltaTime);

    void updatePositions();

    void updateVelocities();

    void addCircles(const std::vector<CircleCollisionComponent *> &circleCollisionComponents);

    void addBoxes(const std::vector<BoxCollisionComponent *> &boxCollisionComponents);

    void cleanCache();

    std::unordered_map<entity, std::pair<b2Body *, Vector2>> bodies;
private:
    PhysicsWrapper();

    static PhysicsWrapper instance;

    std::unique_ptr<ContactListener> contactListener;

    std::unique_ptr<b2World> world;

    static b2BodyType getBodyType(CollisionType collisionType);
};

class ContactListener : public b2ContactListener {
public:
    ContactListener() = default;

    void BeginContact(b2Contact *contact) override;

    void EndContact(b2Contact *contact) override;
};


#endif //BRACK_ENGINE_PHYSICSWRAPPER_HPP
