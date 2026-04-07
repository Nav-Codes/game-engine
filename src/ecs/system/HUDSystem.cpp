//
// Created by navjo on 4/6/2026.
//
#include <sstream>

#include "HUDSystem.hpp"
#include "Component.hpp"
#include "Game.hpp"

void HUDSystem::update(const vector<unique_ptr<Entity> > &entities) {
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

            else if (label.type == LabelType::Win) {
                if (Game::gameState.numEnemies <= 0) {
                    label.visible = true;
                }
            }
        }
    }
}
