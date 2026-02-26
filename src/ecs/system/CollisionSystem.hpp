//
// Created by navjo on 2/4/2026.
//

#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP
#include <memory>
#include <vector>

#include "Entity.hpp"

//forward declaration
class World;

class CollisionSystem {
public:
    void update(World& world);
private:
    std::vector<Entity*> queryCollidables(const std::vector<std::unique_ptr<Entity>>& entities);
};

#endif //COLLISIONSYSTEM_HPP
