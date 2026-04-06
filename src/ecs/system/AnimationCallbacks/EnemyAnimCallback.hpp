//
// Created by navjo on 4/5/2026.
//

#ifndef ENEMYANIMCALLBACK_HPP
#define ENEMYANIMCALLBACK_HPP
#include <string>

#include "Component.hpp"

using namespace std;

class EnemyAnimCallback {
public:
    static string animCallback(Entity& e) {
        if (!e.hasComponent<EnemyAnimationState>()) return nullptr;

        string newClip;

        switch (e.getComponent<EnemyAnimationState>().animState) {
            case EnemyAnimation::Idle :
                newClip = "idle"; break;
            case EnemyAnimation::Walking:
                newClip = "walk"; break;
            case EnemyAnimation::Shooting:
                newClip = "shoot"; break;
            default : newClip = "idle"; break;
        }

        return newClip;
    }
};

#endif //ENEMYANIMCALLBACK_HPP
