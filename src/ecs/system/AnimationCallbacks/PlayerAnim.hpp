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
        if (!e.hasComponent<Velocity>()) {
            return nullptr;
        }
        auto& velocity = e.getComponent<Velocity>();
        auto& anim = e.getComponent<Animation>();

        string newClip;

        if (velocity.direction.x != 0.0f || velocity.direction.y != 0.0f) {
            newClip = "walk";
        } else {
            newClip = "idle";
        }

        // if (velocity.direction.x > 0.0f) {
        //     newClip = anim.oldClip = "fly_right";
        // }
        // else if (velocity.direction.x < 0.0f) {
        //     newClip = anim.oldClip = "fly_left";
        // }
        // else if (velocity.direction.y > 0.0f) {
        //     newClip = anim.oldClip = "fly_down";
        // }
        // else if (velocity.direction.y < 0.0f) {
        //     newClip = anim.oldClip = "fly_up";
        // }
        // else {
        //     if (anim.oldClip == "fly_right") {
        //         newClip = "idle_right";
        //     }
        //     else if (anim.oldClip == "fly_left") {
        //         newClip = "idle_left";
        //     }
        //     else if (anim.oldClip == "fly_up") {
        //         newClip = "idle_up";
        //     }
        //     else if (anim.oldClip == "fly_down") {
        //         newClip = "idle_down";
        //     }
        //     else {
        //         newClip = "idle_right";
        //     }
        // }
        return newClip;
    }
};

#endif //PLAYERANIM_HPP
