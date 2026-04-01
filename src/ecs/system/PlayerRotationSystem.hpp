//
// Created by navjo on 3/31/2026.
//

#ifndef PLAYERROTATIONSYSTEM_HPP
#define PLAYERROTATIONSYSTEM_HPP
#include <memory>
#include <vector>
#include <SDL3/SDL_mouse.h>

#include "Entity.hpp"

using namespace std;

class PlayerRotationSystem {
public:
    void update(vector<unique_ptr<Entity>>& entities) {
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        for (auto& e : entities) {
            // if () {
            //
            // }
            //will use the mouseX and mouseY to create rise/run calculations
            //and plug that into the x and y of the players velocity, since they are floats
            //will not be PlayerMovementSystem because generic MovementSystem covers that for us already

        }
    }
};

#endif //PLAYERROTATIONSYSTEM_HPP
