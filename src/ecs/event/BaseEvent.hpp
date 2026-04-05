//
// Created by navjo on 3/25/2026.
//

#ifndef BASEEVENT_HPP
#define BASEEVENT_HPP

#include "Direction.hpp"
class Entity;

enum class EventType {
    Collision,
    PlayerAction,
    CarAction,
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

enum class CarAction {Accelerate, Decelerate};

struct CarActionEvent : BaseEvent {
    Entity* car = nullptr;
    CarAction action{};
    Direction dir;
    Direction oppositeDir;
    Direction turnDir1;
    Direction turnDir2;
    CarActionEvent(Entity* car, CarAction action) : car(car), action(action) {
        type = EventType::CarAction;
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
