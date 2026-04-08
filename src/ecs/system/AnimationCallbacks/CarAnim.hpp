//
// Created by navjo on 3/23/2026.
//

#ifndef CARANIM_HPP
#define CARANIM_HPP

using namespace std;

#include <string>
#include "Entity.hpp"
#include "Component.hpp"

class CarAnim {
public:
    static string animCallback(Entity& e) {
        if (!e.hasComponent<Velocity>() || !e.hasComponent<Acceleration>()) {
            return nullptr;
        }
        auto& vel = e.getComponent<Velocity>();
        auto& accel = e.getComponent<Acceleration>();
        auto& anim = e.getComponent<Animation>();

        string newClip;

        if (vel.speed == 0.0f) {
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
            else {
                newClip = "stop_facing_east";
            }
        }
        else if (accel.direction == EAST) {
            newClip = anim.oldClip = "drive_east";
        }
        else if (accel.direction == WEST) {
            newClip = anim.oldClip = "drive_west";
        }
        else if (accel.direction == SOUTH) {
            newClip = anim.oldClip = "drive_south";
        }
        else if (accel.direction == NORTH) {
            newClip = anim.oldClip = "drive_north";
        }
        return newClip;
    }
};

#endif //CARANIM_HPP
