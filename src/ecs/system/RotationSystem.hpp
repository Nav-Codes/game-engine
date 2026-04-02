//
// Created by navjo on 3/31/2026.
//

#ifndef ROTATIONSYSTEM_HPP
#define ROTATIONSYSTEM_HPP
#include <cmath>
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
            if (e->hasComponent<Transform>() && e->hasComponent<Sprite>() && e->hasComponent<Rotator>()) {
                auto& t = e->getComponent<Transform>();
                auto& s = e->getComponent<Sprite>();
                auto& r = e->getComponent<Rotator>();

                float targetX, targetY;
                if (r.target == nullptr) {
                    SDL_GetMouseState(&targetX, &targetY);
                } else {
                    targetX = r.target->position.x;
                    targetY = r.target->position.y;
                }

                float startingX = t.position.x + s.dst.w / 2.0f;
                float startingY = t.position.y + s.dst.h / 2.0f;

                float deltaX = targetX - startingX;
                float deltaY = targetY - startingY;

                float angleRadians = atan2(deltaY, deltaX);
                float angleDegrees = angleRadians * 180.0f / M_PI;

                t.rotation = angleDegrees;

                SDL_FPoint center = { s.dst.w / 2.0f, s.dst.h / 2.0f };
                s.center = center;
            }
        }
    }
};

#endif //ROTATIONSYSTEM_HPP
