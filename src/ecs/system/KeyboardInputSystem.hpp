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

            //Car movement controls
            else if (e->hasComponent<CarTag>() && e->hasComponent<Velocity>() && e->hasComponent<Acceleration>() && e->hasComponent<Brake>()) {
                auto& v = e->getComponent<Velocity>();
                auto& a = e->getComponent<Acceleration>();
                auto& b = e->getComponent<Brake>();
                if (event.type == SDL_EVENT_KEY_DOWN) {
                    switch(event.key.key) {
                        case SDLK_W :
                            if ((a.direction == SOUTH && v.speed > ZERO_EPSILON) || (a.direction == NORTH && v.speed < -ZERO_EPSILON)) {
                                a.isAccelerating = false;
                                b.isBraking = true;
                            }
                            else if (a.direction == SOUTH && v.speed == 0.0f) {
                                a.isAccelerating = true;
                                b.isBraking = false;
                                v.speed = -JUMP_START;
                            }
                            else if (a.direction == NORTH || a.direction == EAST || a.direction == WEST) {
                                if (v.speed == 0.0f) {
                                    v.speed = JUMP_START;
                                }
                                a.direction = v.speed > 0.0f ? NORTH : SOUTH;
                                a.isAccelerating = true;
                                b.isBraking = false;
                            }
                            // if (a.direction == EAST || a.direction == WEST) {//do turn animation logic }
                            break;
                        case SDLK_S :
                            if ((a.direction == NORTH && v.speed > ZERO_EPSILON) || (a.direction == SOUTH && v.speed < -ZERO_EPSILON)) {
                                a.isAccelerating = false;
                                b.isBraking = true;
                            }
                            else if (a.direction == NORTH && v.speed == 0.0f) {
                                a.isAccelerating = true;
                                b.isBraking = false;
                                v.speed = -JUMP_START;
                            }
                            else if (a.direction == SOUTH || a.direction == EAST || a.direction == WEST) {
                                if (v.speed == 0.0f) {
                                    v.speed = JUMP_START;
                                }
                                a.direction = v.speed > 0.0f ? SOUTH : NORTH; //the only time this can take effect is when its already south or east or west
                                a.isAccelerating = true;
                                b.isBraking = false;
                            }
                            break;
                        case SDLK_A :
                            if ((a.direction == EAST && v.speed > ZERO_EPSILON) || (a.direction == WEST && v.speed < -ZERO_EPSILON)) {
                                a.isAccelerating = false;
                                b.isBraking = true;
                            }
                            else if (a.direction == EAST && v.speed == 0.0f) {
                                a.isAccelerating = true;
                                b.isBraking = false;
                                v.speed = -JUMP_START;
                            }
                            else if (a.direction == WEST || a.direction == NORTH || a.direction == SOUTH) {
                                if (v.speed == 0.0f) {
                                    v.speed = JUMP_START;
                                }
                                a.direction = v.speed > 0.0f ? WEST : EAST;
                                a.isAccelerating = true;
                                b.isBraking = false;
                            }
                            break;
                        case SDLK_D :
                            if ((a.direction == WEST && v.speed > ZERO_EPSILON) || (a.direction == EAST && v.speed < -ZERO_EPSILON)) {
                                a.isAccelerating = false;
                                b.isBraking = true;
                            }
                            else if (a.direction == WEST && v.speed == 0.0f) {
                                a.isAccelerating = true;
                                b.isBraking = false;
                                v.speed = -JUMP_START;
                            }
                            else if (a.direction == EAST || a.direction == NORTH || a.direction == SOUTH) {
                                if (v.speed == 0.0f) {
                                    v.speed = JUMP_START;
                                }
                                a.direction = v.speed > 0.0f ? EAST : WEST;
                                a.isAccelerating = true;
                                b.isBraking = false;
                            }
                            break;
                        default : break;
                    }
                }
                else if (event.type == SDL_EVENT_KEY_UP) {
                    switch(event.key.key) {
                        case SDLK_W :
                            a.isAccelerating = false;
                            b.isBraking = false;
                            break;
                        case SDLK_S :
                            a.isAccelerating = false;
                            b.isBraking = false;
                            break;
                        case SDLK_A :
                            a.isAccelerating = false;
                            b.isBraking = false;
                            break;
                        case SDLK_D :
                            a.isAccelerating = false;
                            b.isBraking = false;
                            break;
                        default :
                            break;
                    }
                }
            }
        }
    }
};

#endif //KEYBOARDINPUTSYSTEM_HPP
