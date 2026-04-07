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
    void onEnemyCollision(const CollisionEvent& e, const char* otherTag, World& world);

    bool getEntities(const CollisionEvent& e, const char* otherTag, Entity*& focus, Entity*& other, std::string focusedEntity);

};

#endif //COLLISIONRESPONSESYSTEM_HPP
