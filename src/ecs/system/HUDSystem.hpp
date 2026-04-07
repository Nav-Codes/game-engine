//
// Created by navjo on 4/6/2026.
//

#ifndef HUDSYSTEM_HPP
#define HUDSYSTEM_HPP
#include <memory>
#include <vector>

#include "Entity.hpp"

using namespace std;

class HUDSystem {
public:
    void update(const vector<unique_ptr<Entity>>& entities) ;
};

#endif //HUDSYSTEM_HPP
