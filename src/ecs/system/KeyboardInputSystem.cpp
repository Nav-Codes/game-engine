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
                manageDirection(event.key.key, v, a, b);
            }
            else if (event.type == SDL_EVENT_KEY_UP) {
                SDL_Keycode key = event.key.key;
                if (key == SDLK_W || key == SDLK_A || key == SDLK_S || key == SDLK_D) {
                    a.isAccelerating = false;
                    b.isBraking = false;
                }
            }
        }
    }
}

void KeyboardInputSystem::manageDirection(SDL_Keycode key, Velocity& v, Acceleration& a, Brake& b) {
    Direction dir;
    Direction oppositeDir;
    Direction turnDir1;
    Direction turnDir2;

    switch (key) {
        case SDLK_W:
            dir = NORTH;
            oppositeDir = SOUTH;
            turnDir1 = EAST;
            turnDir2 = WEST;
            break;
        case SDLK_S:
            dir = SOUTH;
            oppositeDir = NORTH;
            turnDir1 = EAST;
            turnDir2 = WEST;
            break;
        case SDLK_A:
            dir = WEST;
            oppositeDir = EAST;
            turnDir1 = NORTH;
            turnDir2 = SOUTH;
            break;
        case SDLK_D:
            dir = EAST;
            oppositeDir = WEST;
            turnDir1 = NORTH;
            turnDir2 = SOUTH;
            break;
        default : break;
    }

    if ((a.direction == oppositeDir && v.speed > ZERO_EPSILON) || (a.direction == dir && v.speed < -ZERO_EPSILON)) {
        a.isAccelerating = false;
        b.isBraking = true;
    }
    else if (a.direction == oppositeDir && v.speed == 0.0f) {
        a.isAccelerating = true;
        b.isBraking = false;
        v.speed = -JUMP_START;
    }
    else if (a.direction == dir || a.direction == turnDir1 || a.direction == turnDir2) {
        if (v.speed == 0.0f) {
            v.speed = JUMP_START;
        }
        a.direction = v.speed > 0.0f ? dir : oppositeDir;
        a.isAccelerating = true;
        b.isBraking = false;
    }
}