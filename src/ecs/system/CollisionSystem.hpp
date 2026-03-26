//
// Created by navjo on 2/4/2026.
//

#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include <algorithm>
#include <memory>
#include <set>
#include <vector>

#include "Entity.hpp"

using CollisionKey = std::pair<Entity*, Entity*>;

//forward declaration
class World;

class CollisionSystem {
public:
    void update(World& world);
    std::set<CollisionKey> activeCollisions;
private:
    std::vector<Entity*> queryCollidables(const std::vector<std::unique_ptr<Entity>>& entities);
    CollisionKey makeKey (Entity* entityA, Entity* entityB) {
        return std::minmax(entityA, entityB); //auto orders our pair so smaller num (mem addr) is ordered first
    }
};

#endif //COLLISIONSYSTEM_HPP
