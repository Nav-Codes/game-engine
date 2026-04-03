//
// Created by navjo on 4/1/2026.
//

#ifndef TARGETINGSYSTEM_HPP
#define TARGETINGSYSTEM_HPP
#include <memory>
#include <vector>

#include "Component.hpp"
#include "Entity.hpp"

using namespace std;

class TargetingSystem {
public:
    void update(vector<unique_ptr<Entity>>& entities) {
        for (auto& e : entities) {
            if (e->hasComponent<Transform>() && e->hasComponent<Target>()) {
                auto& transform = e->getComponent<Transform>();
                auto& target = e->getComponent<Target>();

                //this might be affecting how the bullets are being rotated
                float targetX, targetY;
                if (target.target->hasComponent<Camera>()) {
                    SDL_GetMouseState(&targetX, &targetY);
                    targetX += target.target->getComponent<Camera>().view.x;
                    targetY += target.target->getComponent<Camera>().view.y;
                }
                else if (target.target->hasComponent<Transform>()) {
                    targetX = target.target->getComponent<Transform>().position.x + target.targetCenter.x;
                    targetY = target.target->getComponent<Transform>().position.y + target.targetCenter.y;
                }

                float startingX = transform.position.x + target.startingCenter.x;
                float startingY = transform.position.y + target.startingCenter.y;

                float deltaX = targetX - startingX;
                float deltaY = targetY - startingY;

                target.deltaX = deltaX;
                target.deltaY = deltaY;
            }
        }
    }
};

#endif //TARGETINGSYSTEM_HPP
