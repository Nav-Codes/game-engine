//
// Created by navjo on 3/25/2026.
//

#ifndef EVENTRESPONSESYSTEM_HPP
#define EVENTRESPONSESYSTEM_HPP

#include "event/BaseEvent.hpp"

class Entity;
class World;

class EventResponseSystem {
public:
    EventResponseSystem(World& world);

private:
    void onMouseInteraction(const MouseInteractionEvent& e);
};

#endif //EVENTRESPONSESYSTEM_HPP
