//
// Created by navjo on 4/6/2026.
//

#ifndef HUDSYSTEM_HPP
#define HUDSYSTEM_HPP
#include <memory>
#include <vector>
#include <sstream>

#include "Component.hpp"
#include "Entity.hpp"

using namespace std;

class HUDSystem {
public:
    void update(const vector<unique_ptr<Entity>>& entities) {
        Entity* playerEntity = nullptr;

        for (auto& e : entities) {
            if (e->hasComponent<PlayerTag>()) {
                playerEntity = e.get();
                break;
            }
        }

        if (!playerEntity) return;

        auto& playerHealth = playerEntity->getComponent<Health>();

        for (auto& e : entities) {
            if (e->hasComponent<Label>()) {
                auto& label = e.get()->getComponent<Label>();

                //update player position label
                if (label.type == LabelType::Health) {
                    std::stringstream ss;

                    ss << "Health: " << playerHealth.currentHealth;
                    label.text = ss.str();
                    label.dirty = true;

                    if (playerHealth.currentHealth <= 3) {
                        label.color = {255, 0, 0, 255};
                    }
                }
            }
        }
    }
};

#endif //HUDSYSTEM_HPP
