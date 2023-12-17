//
// Created by jesse on 31/10/2023.
//

#include "PhysicsWrapper.hpp"
#include "../includes/ComponentStore.hpp"
#include <vector>
#include <algorithm>
#include <Components/CircleCollisionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/VelocityComponent.hpp>
#include <EngineManagers/SceneManager.hpp>

PhysicsWrapper PhysicsWrapper::instance;

PhysicsWrapper::PhysicsWrapper() {
    b2Vec2 gravity(0.0f, 9.8f);
    world = std::make_unique<b2World>(gravity);
    contactListener = std::make_unique<ContactListener>();
    world->SetContactListener(contactListener.get());
}

PhysicsWrapper::~PhysicsWrapper() = default;

PhysicsWrapper &PhysicsWrapper::getInstance() {
    return instance;
}

PhysicsWrapper::PhysicsWrapper(const PhysicsWrapper &other) {
    b2Vec2 gravity(0.0f, 9.8f);
    world = std::make_unique<b2World>(gravity);
    contactListener = std::make_unique<ContactListener>();
}


void PhysicsWrapper::update(milliseconds deltaTime) {
    updateVelocities();
    const int32 velocityIterations{6};
    const int32 positionIterations{2};
    world->Step(deltaTime, velocityIterations, positionIterations);
    updatePositions();
}


void PhysicsWrapper::addCircles(const std::vector<CircleCollisionComponent *> &circleCollisionComponents) {
    for (auto circle: circleCollisionComponents) {
        auto &bodyPtr = bodies[circle->entityId];
        auto enabled = ComponentStore::GetInstance().tryGetComponent<ObjectInfoComponent>(circle->entityId).isActive &&
                       EntityManager::getInstance().isEntityActive(circle->entityId);
        if (bodyPtr.first == nullptr) {
            auto &transformComp = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(circle->entityId);
            auto &rigidBodyComp = ComponentStore::GetInstance().tryGetComponent<RigidBodyComponent>(circle->entityId);
            b2BodyDef componentBodyDef;
            auto worldPosition = SceneManager::getWorldPosition(transformComp);
            componentBodyDef.position.Set(worldPosition.getX() + circle->offset->getX(),
                                          worldPosition.getY() + circle->offset->getY());
            componentBodyDef.angle = SceneManager::getWorldRotation(transformComp);
            componentBodyDef.type = getBodyType(rigidBodyComp.collisionType);
            componentBodyDef.enabled = enabled;
            bodyPtr.first = world->CreateBody(&componentBodyDef);
            bodyPtr.second = Vector2(circle->offset->getX(),
                                     circle->offset->getY());
            bodyPtr.first->SetGravityScale(rigidBodyComp.gravityScale);

            b2CircleShape shape;
            shape.m_radius = circle->radius * transformComp.scale->getX();

            b2FixtureDef fixtureDef;
            fixtureDef.isSensor = circle->isTrigger;
            fixtureDef.shape = &shape;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = rigidBodyComp.friction;
            fixtureDef.restitution = rigidBodyComp.restitution;

            bodyPtr.first->CreateFixture(&fixtureDef);
        } else {
            auto &transformComp = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(circle->entityId);
            auto worldPosition = SceneManager::getWorldPosition(transformComp);
            bodyPtr.first->SetEnabled(enabled);
            bodyPtr.first->SetTransform(
                    b2Vec2(worldPosition.getX() + bodyPtr.second.getX(),
                           worldPosition.getY() + bodyPtr.second.getY()), 0);
            try {
                auto &velocityComponent = ComponentStore::GetInstance().tryGetComponent<VelocityComponent>(
                        circle->entityId);
                bodyPtr.first->SetLinearVelocity(
                        b2Vec2(velocityComponent.velocity.getX(), velocityComponent.velocity.getY()));
            } catch (std::exception &e) {
                continue;
            }
        }
    }
}

void PhysicsWrapper::addBoxes(const std::vector<BoxCollisionComponent *> &boxCollisionComponents) {
    for (auto box: boxCollisionComponents) {
        auto &bodyPtr = bodies[box->entityId];
        auto enabled = ComponentStore::GetInstance().tryGetComponent<ObjectInfoComponent>(box->entityId).isActive &&
                       EntityManager::getInstance().isEntityActive(box->entityId);
        if (bodyPtr.first == nullptr) {
            auto &transformComp = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(box->entityId);
            auto &rigidBodyComp = ComponentStore::GetInstance().tryGetComponent<RigidBodyComponent>(box->entityId);
            b2BodyDef componentBodyDef;
            auto worldPosition = SceneManager::getWorldPosition(transformComp);
            componentBodyDef.position.Set(worldPosition.getX() + box->offset->getX(),
                                          worldPosition.getY() + box->offset->getY());
            componentBodyDef.angle = SceneManager::getWorldRotation(transformComp);
            componentBodyDef.type = getBodyType(rigidBodyComp.collisionType);
            componentBodyDef.enabled = enabled;
            bodyPtr.first = world->CreateBody(&componentBodyDef);
            bodyPtr.second = Vector2(box->offset->getX(),
                                     box->offset->getY());
            bodyPtr.first->SetGravityScale(rigidBodyComp.gravityScale);

            b2PolygonShape shape;
            shape.SetAsBox(box->size->getX() * transformComp.scale->getX() / 2,
                           box->size->getY() * transformComp.scale->getY() / 2);

            b2FixtureDef fixtureDef;
            fixtureDef.isSensor = box->isTrigger;
            fixtureDef.shape = &shape;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = rigidBodyComp.friction;
            fixtureDef.restitution = rigidBodyComp.restitution;

            bodyPtr.first->CreateFixture(&fixtureDef);
        } else {
            auto &transformComp = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(box->entityId);
            auto worldPosition = SceneManager::getWorldPosition(transformComp);
            bodyPtr.first->SetEnabled(enabled);
            bodyPtr.first->SetTransform(
                    b2Vec2(worldPosition.getX() + bodyPtr.second.getX(),
                           worldPosition.getY() + bodyPtr.second.getY()), 0);
            try {
                auto &velocityComponent = ComponentStore::GetInstance().tryGetComponent<VelocityComponent>(
                        box->entityId);
                bodyPtr.first->SetLinearVelocity(
                        b2Vec2(velocityComponent.velocity.getX(), velocityComponent.velocity.getY()));
            } catch (std::exception &e) {
                continue;
            }
        }
    }
}


b2BodyType PhysicsWrapper::getBodyType(CollisionType collisionType) {
    switch (collisionType) {
        case STATIC:
            return b2_staticBody;
        case DYNAMIC:
            return b2_dynamicBody;
        case KINEMATIC:
            return b2_kinematicBody;
        default:
            return b2_staticBody;
    }
}

void PhysicsWrapper::cleanCache() {
    b2Body *currentBody = world->GetBodyList();
    while (currentBody != nullptr) {
        b2Body *nextBody = currentBody->GetNext();
        world->DestroyBody(currentBody);
        currentBody = nextBody;
    }

    bodies.clear();
}

void PhysicsWrapper::updatePositions() {
    for (auto &body: bodies) {
        try {
            auto &transformComp = ComponentStore::GetInstance().tryGetComponent<TransformComponent>(body.first);
            auto position = body.second.first->GetPosition();
            auto localPosition = SceneManager::getLocalPosition(Vector2(position.x - body.second.second.getX(),
                                                                        position.y - body.second.second.getY()),
                                                                transformComp.entityId);
            transformComp.position->setX(localPosition.getX());
            transformComp.position->setY(localPosition.getY());
        } catch (std::exception &e) {
            continue;
        }
    }
}

void PhysicsWrapper::updateVelocities() {
    for (auto &body: bodies) {
        try {
            auto &transformComp = ComponentStore::GetInstance().tryGetComponent<VelocityComponent>(body.first);
            body.second.first->SetLinearVelocity(b2Vec2(transformComp.velocity.getX(), transformComp.velocity.getY()));
        } catch (std::exception &e) {
            continue;
        }
    }
}


void ContactListener::BeginContact(b2Contact *contact) {
    std::unordered_map<entity, std::pair<b2Body *, Vector2>> bodies = PhysicsWrapper::getInstance().bodies;
    auto &compStore = ComponentStore::GetInstance();
    auto contactA = contact->GetFixtureA()->GetBody();
    auto contactB = contact->GetFixtureB()->GetBody();
    auto circleEntities = compStore.getEntitiesWithComponent<CircleCollisionComponent>();
    auto boxEntities = compStore.getEntitiesWithComponent<BoxCollisionComponent>();

    entity contactAComponent;
    entity contactBComponent;

    for (auto body: bodies) {
        if (body.second.first == contactA) {
            contactAComponent = body.first;
        }
        if (body.second.first == contactB) {
            contactBComponent = body.first;
        }

    }
    // Add ContactB to ContactA Circle
    if (std::find(circleEntities.begin(), circleEntities.end(), contactAComponent) != circleEntities.end()) {
        compStore.tryGetComponent<CircleCollisionComponent>(contactAComponent).collidedWith.push_back(
                contactBComponent);
    }
    // Add ContactA to ContactB Circle
    if (std::find(circleEntities.begin(), circleEntities.end(), contactBComponent) != circleEntities.end()) {
        compStore.tryGetComponent<CircleCollisionComponent>(contactBComponent).collidedWith.push_back(
                contactAComponent);
    }
    // Add ContactB to ContactA Box
    if (std::find(boxEntities.begin(), boxEntities.end(), contactAComponent) != boxEntities.end()) {
        compStore.tryGetComponent<BoxCollisionComponent>(contactAComponent).collidedWith.push_back(contactBComponent);
    }
    // Add ContactA to ContactB Box
    if (std::find(boxEntities.begin(), boxEntities.end(), contactBComponent) != boxEntities.end()) {
        compStore.tryGetComponent<BoxCollisionComponent>(contactBComponent).collidedWith.push_back(contactAComponent);
    }

}


void ContactListener::EndContact(b2Contact *contact) {
    std::unordered_map<entity, std::pair<b2Body *, Vector2>> bodies = PhysicsWrapper::getInstance().bodies;
    auto &compStore = ComponentStore::GetInstance();
    auto contactA = contact->GetFixtureA()->GetBody();
    auto contactB = contact->GetFixtureB()->GetBody();
    auto circleEntities = compStore.getEntitiesWithComponent<CircleCollisionComponent>();
    auto boxEntities = compStore.getEntitiesWithComponent<BoxCollisionComponent>();

    entity contactAComponent;
    entity contactBComponent;

    for (auto body: bodies) {
        if (body.second.first == contactA) {
            contactAComponent = body.first;
        }
        if (body.second.first == contactB) {
            contactBComponent = body.first;
        }

    }
    if (std::find(circleEntities.begin(), circleEntities.end(), contactAComponent) != circleEntities.end()) {
        auto &component = compStore.tryGetComponent<CircleCollisionComponent>(contactAComponent);
        component.collidedWith.erase(
                std::remove(component.collidedWith.begin(), component.collidedWith.end(), contactBComponent),
                component.collidedWith.end());
    }
    if (std::find(circleEntities.begin(), circleEntities.end(), contactBComponent) != circleEntities.end()) {
        auto &component = compStore.tryGetComponent<CircleCollisionComponent>(contactBComponent);
        component.collidedWith.erase(
                std::remove(component.collidedWith.begin(), component.collidedWith.end(), contactAComponent),
                component.collidedWith.end());
    }
    if (std::find(boxEntities.begin(), boxEntities.end(), contactAComponent) != boxEntities.end()) {
        auto &component = compStore.tryGetComponent<BoxCollisionComponent>(contactAComponent);
        component.collidedWith.erase(
                std::remove(component.collidedWith.begin(), component.collidedWith.end(), contactBComponent),
                component.collidedWith.end());
    }
    if (std::find(boxEntities.begin(), boxEntities.end(), contactBComponent) != boxEntities.end()) {
        auto &component = compStore.tryGetComponent<BoxCollisionComponent>(contactBComponent);
        component.collidedWith.erase(
                std::remove(component.collidedWith.begin(), component.collidedWith.end(), contactAComponent),
                component.collidedWith.end());
    }


}
