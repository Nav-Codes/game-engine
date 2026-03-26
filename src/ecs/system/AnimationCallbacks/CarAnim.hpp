//
// Created by navjo on 3/23/2026.
//

#ifndef CARANIM_HPP
#define CARANIM_HPP

using namespace std;

#include <string>
#include "Entity.hpp"
#include "Component.hpp"

//will need to keep track of
//on D keypress or A keypress, stop current animation loop and create a
//for idle animation, can just leave it as the last directional animation

class CarAnim {
public:
    static string animCallback(Entity& e) {
        if (!e.hasComponent<Velocity>() || !e.hasComponent<Acceleration>()) {
            return nullptr;
        }
        auto& vel = e.getComponent<Velocity>();
        auto& a = e.getComponent<Acceleration>();
        auto& anim = e.getComponent<Animation>();

        string newClip;

        if (a.direction == EAST) {
            newClip = anim.oldClip = "drive_east";
        }
        else if (a.direction == WEST) {
            newClip = anim.oldClip = "drive_west";
        }
        else if (a.direction == SOUTH) {
            newClip = anim.oldClip = "drive_south";
        }
        else if (a.direction == NORTH) {
            newClip = anim.oldClip = "drive_north";
        }
        else if (a.direction == STOP) {
            if (anim.oldClip == "drive_east") {
                newClip = "stop_facing_east";
            }
            else if (anim.oldClip == "drive_west") {
                newClip = "stop_facing_west";
            }
            else if (anim.oldClip == "drive_north") {
                newClip = "stop_facing_north";
            }
            else if (anim.oldClip == "drive_south") {
                newClip = "stop_facing_south";
            }
        }
        return newClip;
    }
};

#endif //CARANIM_HPP
