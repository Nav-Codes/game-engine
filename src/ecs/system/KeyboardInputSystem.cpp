//
// Created by navjo on 4/3/2026.
//

#include "KeyboardInputSystem.hpp"
#include "World.hpp"

//changes direction of entity
void KeyboardInputSystem::update(const std::vector<std::unique_ptr<Entity> > &entities, const SDL_Event &event, World& world) {
    for (auto& e : entities) {
        //global controls
        if (e->hasComponent<Interactable>()) {
            auto& i = e->getComponent<Interactable>();
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_E && i.interactable)
                world.getEventManager().emit(PlayerActionEvent{e.get(), PlayerAction::CarInteract});
        }

        //player controls
        if (e->hasComponent<PlayerTag>() && e->hasComponent<Velocity>() && e->hasComponent<PlayerAnimationState>()) {
            if (e->hasComponent<KeyboardFocusTag>())
                if (!e->getComponent<KeyboardFocusTag>().active)
                    continue;

            auto& v = e->getComponent<Velocity>();
            auto& ps = e->getComponent<PlayerAnimationState>();
            if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                switch(event.key.key) {
                    case SDLK_W :
                        v.direction.y = -1;
                        ps.W = true;
                        break;
                    case SDLK_S :
                        v.direction.y = 1;
                        ps.S = true;
                        break;
                    case SDLK_A :
                        v.direction.x = -1;
                        ps.A = true;
                        break;
                    case SDLK_D :
                        v.direction.x = 1;
                        ps.D = true;
                        break;
                    default : break;
                }
                if ((ps.W || ps.A || ps.S || ps.D) && ps.animState != PlayerAnimation::Shooting) {
                    ps.animState = PlayerAnimation::Walking;
                }
                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    ps.animState = PlayerAnimation::Shooting;
                }
            }
            if (event.type == SDL_EVENT_KEY_UP || event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                switch(event.key.key) {
                    case SDLK_W :
                        v.direction.y = 0;
                        ps.W = false;
                        break;
                    case SDLK_S :
                        v.direction.y = 0;
                        ps.S = false;
                        break;
                    case SDLK_A :
                        v.direction.x = 0;
                        ps.A = false;
                        break;
                    case SDLK_D :
                        v.direction.x = 0;
                        ps.D = false;
                        break;
                    default : break;
                }
                if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    if (!ps.W && !ps.A && !ps.S && !ps.D) {
                        ps.animState = PlayerAnimation::Idle;
                    } else {
                        ps.animState = PlayerAnimation::Walking;
                    }
                }
                else if (!ps.W && !ps.A && !ps.S && !ps.D && ps.animState != PlayerAnimation::Shooting) {
                    ps.animState = PlayerAnimation::Idle;
                }
            }
        }

        //Car movement controls
        else if (e->hasComponent<CarTag>() && e->hasComponent<Velocity>() && e->hasComponent<Acceleration>() && e->hasComponent<Brake>()) {
            if (e->hasComponent<KeyboardFocusTag>())
                if (!e->getComponent<KeyboardFocusTag>().active)
                    continue;
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
                            a.direction = v.speed > 0.0f ? SOUTH : NORTH;
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
