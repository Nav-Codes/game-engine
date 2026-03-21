//
// Created by navjo on 3/18/2026.
//

#ifndef PROJECTILECALLBACK_HPP
#define PROJECTILECALLBACK_HPP

using namespace std;

#include <string>

class ProjectileCallback {
public:
    static string animCallback(Entity& e) {
        return "idle_right";
    }
};

#endif //PROJECTILECALLBACK_HPP
