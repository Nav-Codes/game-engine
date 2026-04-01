//
// Created by navjo on 3/31/2026.
//

#ifndef PLAYERROTATIONSYSTEM_HPP
#define PLAYERROTATIONSYSTEM_HPP
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include <SDL3/SDL_mouse.h>

#include "Component.hpp"
#include "Entity.hpp"

using namespace std;

class PlayerRotationSystem {
public:
    void update(vector<unique_ptr<Entity>>& entities) {
        for (auto& e : entities) {
            if (e->hasComponent<PlayerTag>() && e->hasComponent<Velocity>() && e->hasComponent<Transform>() && e->hasComponent<Sprite>()) {
                auto& v = e->getComponent<Velocity>();
                auto& t = e->getComponent<Transform>();
                auto& s = e->getComponent<Sprite>();

                float mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                float playerX = t.position.x + s.dst.w / 2.0f;
                float playerY = t.position.y + s.dst.h / 2.0f;

                float deltaX = mouseX - playerX;
                float deltaY = mouseY - playerY;

                float angleRadians = atan2(deltaY, deltaX);
                float angleDegrees = angleRadians * 180.0f / M_PI;

                t.rotation = angleDegrees;

                SDL_FPoint center = { s.dst.w / 2.0f, s.dst.h / 2.0f };
                s.center = center;
            }
        }
    }
};

#endif //PLAYERROTATIONSYSTEM_HPP
