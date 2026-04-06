//
// Created by navjo on 4/5/2026.
//

#ifndef COLLISIONRESPONSESYSTEM_HPP
#define COLLISIONRESPONSESYSTEM_HPP
#include "event/BaseEvent.hpp"

class World;

class CollisionResponseSystem {
public:
    CollisionResponseSystem(World& world);

    void onCollision(const CollisionEvent &e, const char *otherTag, World &world);
    bool getCollisionEntities(const CollisionEvent& e, const char* otherTag, Entity*& player, Entity*& other);
};

#endif //COLLISIONRESPONSESYSTEM_HPP
