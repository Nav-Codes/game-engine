//
// Created by navjo on 4/1/2026.
//

#ifndef TARGETINGSYSTEM_HPP
#define TARGETINGSYSTEM_HPP
#include <memory>
#include <vector>

#include "Component.hpp"
#include "Entity.hpp"

using namespace std;


//this will essentially loop thru all the entities with the Targeting component
//it will calculate the rise/run stuff and store it in the component
//purpose of this is to do the targeting system calculations, so that
//turrets and guns can just use that to define x and y velocities for their projectiles
class TargetingSystem {
public:
    void update(vector<unique_ptr<Entity>>& entities) {
        for (auto& e : entities) {
            if (e->hasComponent<Targeting>()) {
                //calculate the targeting thing for it
                //get the start transform and the target transform
                //calculate the rise/run from start to target
                //rise = y2-y1, y2=target, y1=start; same thing for x2 and x1
                //ensure that rise/run values are less than 1 if entity is not moving up, down, left, or right
                //store that in the Targeting component
            }
        }
    }
};

#endif //TARGETINGSYSTEM_HPP
