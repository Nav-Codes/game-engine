//
// Created by navjo on 3/25/2026.
//
#include "EventResponseSystem.hpp"

#include "World.hpp"

EventResponseSystem::EventResponseSystem(World &world) {
    //subscriptions
    world.getEventManager().subscribe([this, &world](const BaseEvent& e) {
        if (e.type != EventType::MouseInteraction) return;
        const auto& mouseInteractionEvent = static_cast<const MouseInteractionEvent&>(e);
        onMouseInteraction(mouseInteractionEvent);
    });
}

void EventResponseSystem::onMouseInteraction(const MouseInteractionEvent &e) {
    if (!e.entity->hasComponent<Clickable>()) return;

    auto& clickable = e.entity->getComponent<Clickable>();

    switch (e.state) {
        case MouseInteractionState::Pressed :
            clickable.onPressed();
            break;
        case MouseInteractionState::Released :
            clickable.onReleased();
            break;
        case MouseInteractionState::Cancel:
            clickable.onCancel();
            break;
        default : break;
    }
}
