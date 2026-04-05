//
// Created by navjo on 1/28/2026.
//

#ifndef KEYBOARDINPUTSYSTEM_HPP
#define KEYBOARDINPUTSYSTEM_HPP
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Component.hpp"
class World;

class KeyboardInputSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event& event, World& world);
private:
    void manageDirection(SDL_Keycode key, Velocity& v, Acceleration& a, Brake& b);
};

#endif //KEYBOARDINPUTSYSTEM_HPP
