//
// Created by navjo on 1/21/2026.
//

#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP
#include <memory>
#include <vector>

#include "Component.hpp"
#include "Entity.hpp"
#include "utils/Vector2D.hpp"

class MovementSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& entity : entities) {
            if (entity -> hasComponent<Transform>() && entity -> hasComponent<Velocity>()) {
                auto& t = entity->getComponent<Transform>();
                auto& v = entity->getComponent<Velocity>();

                //track prev frames position
                t.oldPosition = t.position;

                Vector2D directionVec = v.direction;
                //normalizing (hasn't happened yet)
                directionVec.normalize();
                //Vector2D needs operator function to multiply a float to itself
                Vector2D velocityVector = directionVec * v.speed;

                t.position += (dt * velocityVector);

            }
        }
    }
};

#endif //MOVEMENT_HPP
