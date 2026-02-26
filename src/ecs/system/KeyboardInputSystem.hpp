//
// Created by navjo on 1/28/2026.
//

#ifndef KEYBOARDINPUTSYSTEM_HPP
#define KEYBOARDINPUTSYSTEM_HPP
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Component.hpp"
#include "Entity.hpp"

class KeyboardInputSystem {
public:

    //changes direction of entity
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event& event) {
        for (auto& e : entities) {
            if (e->hasComponent<PlayerTag>() && e->hasComponent<Velocity>()) {
                auto& v = e->getComponent<Velocity>();
                if (event.type == SDL_EVENT_KEY_DOWN) {
                    switch(event.key.key) {
                        case SDLK_W :
                            v.direction.y = -1; break;
                        case SDLK_S :
                            v.direction.y = 1; break;
                        case SDLK_A :
                            v.direction.x = -1; break;
                        case SDLK_D :
                            v.direction.x = 1; break;
                        default : break;
                    }
                }
                    if (event.type == SDL_EVENT_KEY_UP) {
                        switch(event.key.key) {
                            case SDLK_W :
                                v.direction.y = 0; break;
                            case SDLK_S :
                                v.direction.y = 0; break;
                            case SDLK_A :
                                v.direction.x = 0; break;
                            case SDLK_D :
                                v.direction.x = 0; break;
                            default : break;
                        }
                    }
                }
            }
        }
};

#endif //KEYBOARDINPUTSYSTEM_HPP


// auto& t = entity->getComponent<Transform>();
// t.position.x += 60 * dt;
// t.position.x += 60 * dt;