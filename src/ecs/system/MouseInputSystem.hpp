//
// Created by navjo on 3/31/2026.
//

#ifndef MOUSEINPUTSYSTEM_HPP
#define MOUSEINPUTSYSTEM_HPP
#include <SDL3/SDL_events.h>

class World;

class MouseInputSystem {
public:
    void update(World& world, const SDL_Event event);
};

#endif //MOUSEINPUTSYSTEM_HPP
