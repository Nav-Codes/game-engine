//
// Created by navjo on 1/21/2026.
//

#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#include <memory>
#include <vector>

#include "Component.hpp"
#include "Entity.hpp"
#include "TextureManager.hpp"

class RenderSystem {
public:
    void render(std::vector<std::unique_ptr<Entity>>& entities) {

        Entity* cameraEntity = nullptr;
        for (auto& e : entities) {
            if(e->hasComponent<Camera>()) {
                cameraEntity = e.get();
                break;
            }
        }

        if (!cameraEntity) {
            return;
        }

        auto& cam = cameraEntity->getComponent<Camera>();

        for (auto& entity : entities) {
            if (entity->hasComponent<Transform>() && entity->hasComponent<Sprite>()) {
                auto& t = entity->getComponent<Transform>();
                auto& sprite = entity->getComponent<Sprite>();

                //we are converting from world space to screen space
                sprite.dst.x = t.position.x - cam.view.x;
                sprite.dst.y = t.position.y - cam.view.y;

                //if entity has an animation component update src rect
                if(entity->hasComponent<Animation>()) {
                    auto& anim = entity->getComponent<Animation>();
                    sprite.src = anim.clips[anim.currentClip].frameIndices[anim.currentFrame];
                }

                TextureManager::draw(sprite.texture, sprite.src, sprite.dst);
            }
        }
    }
};

#endif //RENDERSYSTEM_HPP
