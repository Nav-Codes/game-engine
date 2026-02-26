//
// Created by navjo on 2/25/2026.
//

#ifndef DESTRUCTIONSYSTEM_HPP
#define DESTRUCTIONSYSTEM_HPP
#include <memory>
#include <vector>

#include "Component.hpp"
#include "Entity.hpp"

using namespace std;

class DestructionSystem {
public:
    void update (const vector<unique_ptr<Entity>>& entities) {
        Entity * cameraEntity = nullptr;

        //find camera
        for (auto& e : entities) {
            if (e->hasComponent<Camera>()) {
                cameraEntity = e.get();
                break;
            }
        }

        if (!cameraEntity) return;
        auto& cam = cameraEntity->getComponent<Camera>();

        for (auto& e : entities) {
            if (e->hasComponent<Transform>() && e->hasComponent<ProjectileTag>()) {
                auto& t = e->getComponent<Transform>();

                //destroy entity if it goes out the cam view

                if (t.position.x > cam.view.x + cam.view.w || //right edge of view
                    t.position.x < cam.view.x || //top edge
                    t.position.y > cam.view.y + cam.view.h || //bottom
                    t.position.y < cam.view.y) { //top
                    e->destroy();
                }
            }
        }
    }
};

#endif //DESTRUCTIONSYSTEM_HPP
