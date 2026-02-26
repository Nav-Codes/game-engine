//
// Created by navjo on 2/4/2026.
//

#ifndef COLLISION_HPP
#define COLLISION_HPP
#include <SDL3/SDL_rect.h>
#include "Component.hpp"

class Collision {
public:
    static bool AABB(const SDL_FRect& rectA, const SDL_FRect& rectB) ;
    static bool AABB(const Collider& colA, const Collider& colB);

};

#endif //COLLISION_HPP
