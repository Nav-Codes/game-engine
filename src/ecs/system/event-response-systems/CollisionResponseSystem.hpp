//
// Created by navjo on 4/5/2026.
//

#ifndef COLLISIONRESPONSESYSTEM_HPP
#define COLLISIONRESPONSESYSTEM_HPP
#include <string>

#include "event/BaseEvent.hpp"

class World;

class CollisionResponseSystem {
public:
    CollisionResponseSystem(World& world);

    // void onCollision(const CollisionEvent &e, const char *otherTag, World &world);
    void onPlayerCollision(const CollisionEvent& e, const char* otherTag, World& world);
    void onCarCollision(const CollisionEvent& e, const char* otherTag, World& world);

    //can be used to generalize wall collisions, since its mostly the same for all entities
    // void onWallCollision(const CollisionEvent& e, const char* otherTag, World& world);

    // bool getCollisionEntities(const CollisionEvent& e, const char* otherTag, Entity*& player, Entity*& other);
    // bool getPlayerEntities(const CollisionEvent& e, const char* otherTag, Entity*& player, Entity*& other);
    // bool getCarEntities(const CollisionEvent& e, const char* otherTag, Entity*& car, Entity*& other);
    bool getEntities(const CollisionEvent& e, const char* otherTag, Entity*& focus, Entity*& other, std::string focusedEntity);

};

#endif //COLLISIONRESPONSESYSTEM_HPP
