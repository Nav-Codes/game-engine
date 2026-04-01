//
// Created by navjo on 3/31/2026.
//
#include "MouseInputSystem.hpp"

#include "World.hpp"

void MouseInputSystem::update(World &world, const SDL_Event event) {
    if (event.type != SDL_EVENT_MOUSE_MOTION &&
        event.type != SDL_EVENT_MOUSE_BUTTON_UP &&
        event.type != SDL_EVENT_MOUSE_BUTTON_DOWN)
        return;

    float mx, my;
    SDL_GetMouseState(&mx, &my);

    for (auto& e : world.getEntities()) {
        if (e->hasComponent<Clickable>() && e->hasComponent<Collider>()) {
            Clickable& clickable = e->getComponent<Clickable>();
            Collider& collider = e->getComponent<Collider>();

            if (!collider.enabled) continue;

            bool inside = (mx >= collider.rect.x && mx <= collider.rect.x + collider.rect.w &&
                my >= collider.rect.y && my <= collider.rect.y + collider.rect.h);

            //hover
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                if (!inside && clickable.pressed) {
                    world.getEventManager().emit(MouseInteractionEvent{e.get(), MouseInteractionState::Cancel});
                    ///cancelled
                }
            }

            //pressed
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (inside) {
                        clickable.pressed = true;
                        world.getEventManager().emit(MouseInteractionEvent{e.get(), MouseInteractionState::Pressed});
                        //pressed event
                    }
                }
            }

            //released
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (inside) {
                        clickable.pressed = false;
                        world.getEventManager().emit(MouseInteractionEvent{e.get(), MouseInteractionState::Released});
                        //released event
                    }
                }
            }
        }
    }
}
