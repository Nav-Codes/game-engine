//
// Created by navjo on 3/25/2026.
//

#ifndef EVENTRESPONSESYSTEM_HPP
#define EVENTRESPONSESYSTEM_HPP

#include "event/BaseEvent.hpp"

class Entity;
class World;

class MouseUIResponseSystem {
public:
    MouseUIResponseSystem(World& world);

private:
    void onMouseInteraction(const MouseInteractionEvent& e);
};

#endif //EVENTRESPONSESYSTEM_HPP
