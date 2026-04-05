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
        else if (e->hasComponent<CarTag>() && e->hasComponent<KeyboardFocusTag>() && e->getComponent<KeyboardFocusTag>().active) {
            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_W: {
                        CarActionEvent north{CarActionEvent{e.get(), CarAction::Accelerate}};
                        north.dir = NORTH;
                        north.oppositeDir = SOUTH;
                        north.turnDir1 = EAST;
                        north.turnDir2 = WEST;
                        world.getEventManager().emit(CarActionEvent(north));
                        break;
                    }
                    case SDLK_S: {
                        CarActionEvent south{CarActionEvent{e.get(), CarAction::Accelerate}};
                        south.dir = SOUTH;
                        south.oppositeDir = NORTH;
                        south.turnDir1 = EAST;
                        south.turnDir2 = WEST;
                        world.getEventManager().emit(CarActionEvent(south));
                        break;
                    }
                    case SDLK_A: {
                        CarActionEvent west{CarActionEvent{e.get(), CarAction::Accelerate}};
                        west.dir = WEST;
                        west.oppositeDir = EAST;
                        west.turnDir1 = NORTH;
                        west.turnDir2 = SOUTH;
                        world.getEventManager().emit(CarActionEvent(west));
                        break;
                    }
                    case SDLK_D: {
                        CarActionEvent east{CarActionEvent{e.get(), CarAction::Accelerate}};
                        east.dir = EAST;
                        east.oppositeDir = WEST;
                        east.turnDir1 = NORTH;
                        east.turnDir2 = SOUTH;
                        world.getEventManager().emit(CarActionEvent(east));
                        break;
                    }
                    default : break;
                }
            }
            else if (event.type == SDL_EVENT_KEY_UP) {
                SDL_Keycode key = event.key.key;
                if (key == SDLK_W || key == SDLK_A || key == SDLK_S || key == SDLK_D) {

                    world.getEventManager().emit(CarActionEvent{e.get(), CarAction::Decelerate});
                }
            }
        }
    }
}