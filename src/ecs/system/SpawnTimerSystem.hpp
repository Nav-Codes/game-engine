//
// Created by navjo on 2/25/2026.
//

#ifndef SPAWNTIMERSYSTEM_HPP
#define SPAWNTIMERSYSTEM_HPP
#include <memory>
#include <vector>

#include "Component.hpp"
#include "Entity.hpp"

using namespace std;

class SpawnTimerSystem {
public:
    void update(const vector<unique_ptr<Entity>>& entities, const float dt) {
        for (auto& entity : entities) {
            if (entity->hasComponent<TimedSpawner>()) {
                auto& spawner = entity->getComponent<TimedSpawner>();

                spawner.timer -=dt;

                if (spawner.timer <= 0) {
                    spawner.timer = spawner.spawnInterval; // eg. 2 sec
                    spawner.spawnCallback(); //create entity in here
                }
            }
        }
    }
};

#endif //SPAWNTIMERSYSTEM_HPP
