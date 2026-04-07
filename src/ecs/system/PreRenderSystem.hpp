//
// Created by navjo on 4/6/2026.
//

#ifndef PRERENDERSYSTEM_HPP
#define PRERENDERSYSTEM_HPP
#include <memory>
#include <vector>

#include "Component.hpp"
#include "Entity.hpp"
#include "../../manager/TextureManager.hpp"

using namespace std;

class PreRenderSystem {
public:
    void update(const vector<unique_ptr<Entity>>& entities) {
        for (auto& entity : entities) {
            if (entity->hasComponent<Label>() && entity->getComponent<Label>().dirty) {
                //update label texture
                TextureManager::updateLabel(entity->getComponent<Label>());
            }
        }
    }
};

#endif //PRERENDERSYSTEM_HPP
