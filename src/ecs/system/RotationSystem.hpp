//
// Created by navjo on 3/31/2026.
//

#ifndef ROTATIONSYSTEM_HPP
#define ROTATIONSYSTEM_HPP
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include <SDL3/SDL_mouse.h>

#include "Component.hpp"
#include "Entity.hpp"

using namespace std;

class RotationSystem {
public:
    void update(vector<unique_ptr<Entity>>& entities) {
        for (auto& e : entities) {
            if (e->hasComponent<Transform>() && e->hasComponent<Sprite>() && e->hasComponent<Target>()) {
                auto& transform = e->getComponent<Transform>();
                auto& target = e->getComponent<Target>();

                float angleRadians = atan2(target.deltaY, target.deltaX);
                float angleDegrees = angleRadians * 180.0f / M_PI;

                transform.rotation = angleDegrees;
            }
        }
    }
};

#endif //ROTATIONSYSTEM_HPP
