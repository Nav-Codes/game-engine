//
// Created by navjo on 3/24/2026.
//

#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP

using namespace std;
#include <vector>
#include <memory>

#include "Entity.hpp"
#include "Component.hpp"

class PhysicsSystem {
public:
    void update(const vector<unique_ptr<Entity>>& entities, float dt) {
        for (auto& e : entities) {
            if (e->hasComponent<Velocity>() && e->hasComponent<Acceleration>() && e->hasComponent<Brake>()) {
                //this essentially determines which way we move and how quickly we speed up
                auto& v = e->getComponent<Velocity>();
                auto& a = e->getComponent<Acceleration>();
                auto& b = e->getComponent<Brake>();

                //Ensure that the object is facing in the correct direction
                switch (a.direction) {
                    case NORTH :
                        v.direction.x = 0;
                        v.direction.y = -1;
                        break;
                    case EAST :
                        v.direction.x = 1;
                        v.direction.y = 0;
                        break;
                    case SOUTH :
                        v.direction.x = 0;
                        v.direction.y = 1;
                        break;
                    case WEST :
                        v.direction.x = -1;
                        v.direction.y = 0;
                        break;
                    default : break;
                }

                //decelerating
                if (!a.isAccelerating) {
                    if (v.speed < ZERO_EPSILON && v.speed > -ZERO_EPSILON) { //if speed is 0
                        v.speed = 0.0f;
                    }
                    else {
                        float accelFactor = b.isBraking ? a.accelerationFactor * b.brakeForce : a.accelerationFactor;
                        accelFactor = v.speed < 0.0f ? -accelFactor : accelFactor;
                        v.speed -= accelFactor * dt;
                    }
                }
                //accelerating
                else if (a.isAccelerating) {
                    //change to account for +ve speeds
                    v.speed += (v.speed <0.0f ? -a.accelerationFactor : a.accelerationFactor) * dt;

                    if (v.speed >= v.maxSpeed) {
                        v.speed = v.maxSpeed;
                    }
                    if (v.speed <= -v.maxSpeed) {
                        v.speed = -v.maxSpeed;
                    }
                }
            }
        }
    }
};

#endif //PHYSICSSYSTEM_HPP
