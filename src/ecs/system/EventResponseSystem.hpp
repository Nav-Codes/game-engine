//
// Created by navjo on 3/25/2026.
//

#ifndef EVENTRESPONSESYSTEM_HPP
#define EVENTRESPONSESYSTEM_HPP
#include <functional>

#include "event/BaseEvent.hpp"

class Entity;
class World;

class EventResponseSystem {
public:
    EventResponseSystem(World& world);

private:
    //collisions
    void onCollision(const CollisionEvent& e, const char* otherTag, World& world);
    bool getCollisionEntities(const CollisionEvent& e, const char* otherTag, Entity*& player, Entity*& other);

    //player action
    void onPlayerAction(const PlayerActionEvent& e, const std::function<void(Entity* player, PlayerAction action)>& callback);
};

#endif //EVENTRESPONSESYSTEM_HPP
