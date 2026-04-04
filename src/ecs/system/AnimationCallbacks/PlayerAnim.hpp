//
// Created by navjo on 3/18/2026.
//

#ifndef PLAYERANIM_HPP
#define PLAYERANIM_HPP

using namespace std;

#include <string>
#include "Component.hpp"

class PlayerAnim {
public:
    static string animCallback(Entity& e) {
        //we already check if the entity has a velocity and animation component
        //so no need to check again here
        if (!e.hasComponent<PlayerAnimationState>()) return nullptr;

        string newClip;

        switch (e.getComponent<PlayerAnimationState>().animState) {
            case PlayerAnimation::Idle :
                newClip = "idle"; break;
            case PlayerAnimation::Walking:
                newClip = "walk"; break;
            case PlayerAnimation::Shooting:
                newClip = "shoot"; break;
            default : newClip = "idle"; break;
        }

        return newClip;
    }
};

#endif //PLAYERANIM_HPP
