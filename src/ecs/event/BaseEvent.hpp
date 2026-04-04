//
// Created by navjo on 3/25/2026.
//

#ifndef BASEEVENT_HPP
#define BASEEVENT_HPP

class Entity;

enum class EventType {
    Collision,
    PlayerAction,
    MouseInteraction
};

struct BaseEvent {
    EventType type{};
    virtual ~BaseEvent() = default;
};

enum class CollisionState {Enter, Stay, Exit};

struct CollisionEvent : BaseEvent {
    Entity* entityA = nullptr;
    Entity* entityB = nullptr;
    CollisionState state{};
    CollisionEvent(Entity* entityA, Entity* entityB, CollisionState state) : entityA(entityA), entityB(entityB), state(state) {
        type = EventType::Collision;
    }
};

enum class PlayerAction {Shoot, Walk, Idle, CarInteract};

struct PlayerActionEvent : BaseEvent {
    Entity* player = nullptr;
    PlayerAction action{};
    PlayerActionEvent(Entity* player, PlayerAction action) : player(player), action(action) {
        type = EventType::PlayerAction;
    }
};

enum class MouseInteractionState {Pressed, Released, Cancel};

struct MouseInteractionEvent : BaseEvent {
    Entity* entity = nullptr;
    MouseInteractionState state{};
    MouseInteractionEvent(Entity* entity, MouseInteractionState state) : entity(entity), state(state) {
        type = EventType::MouseInteraction;
    }
};

#endif //BASEEVENT_HPP
