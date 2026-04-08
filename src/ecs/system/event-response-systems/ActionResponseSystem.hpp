//
// Created by navjo on 4/5/2026.
//

#ifndef ACTIONRESPONSESYSTEM_HPP
#define ACTIONRESPONSESYSTEM_HPP

#include "event/BaseEvent.hpp"

class World;

class ActionResponseSystem {
public:
    ActionResponseSystem(World& world);

    void onPlayerAction(const PlayerActionEvent& e, World& world);
    void onCarAction(const CarActionEvent& e);
};

#endif //ACTIONRESPONSESYSTEM_HPP
