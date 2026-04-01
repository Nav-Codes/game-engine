//
// Created by navjo on 3/31/2026.
//

#ifndef UIRENDERSYSTEM_HPP
#define UIRENDERSYSTEM_HPP

using namespace std;

#include <memory>
#include <vector>

#include "Component.hpp"
#include "Entity.hpp"
#include "RenderUtils.hpp"
#include "TextureManager.hpp"

class UIRenderSystem {
public:
    void render(const vector<unique_ptr<Entity>>& entities) {
        for (auto& e : entities) {
            if (e->hasComponent<Transform>() && e->hasComponent<Sprite>()) {
                auto& transform = e->getComponent<Transform>();
                auto& sprite = e->getComponent<Sprite>();

                if (sprite.renderLayer != RenderLayer::UI) continue;

                sprite.dst.x = transform.position.x;
                sprite.dst.y = transform.position.y;

                if (sprite.visible) {
                    SDL_FRect scaledDest = RenderUtils::getScaledDest(sprite.dst, transform.scale);
                    TextureManager::draw(sprite.texture, sprite.src, scaledDest);
                }
            }
        }
    }
};

#endif //UIRENDERSYSTEM_HPP
