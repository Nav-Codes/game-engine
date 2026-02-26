//
// Created by navjo on 2/25/2026.
//

#ifndef SCENE_HPP
#define SCENE_HPP
#include <string>
#include <SDL3/SDL_events.h>

// #include "Vector2D.hpp"
#include "World.hpp"

using namespace std;

class Scene {
public:
    World world;

    const string& getName() const {return name;}

    Scene(const char* sceneName, const char* mapPath, int windowWidth, int windowHeight);

    void update(const float dt, const SDL_Event& e) {
        world.update(dt, e);
    }

    void render() {
        world.render();
    }

private:
    string name;
    void createProjectile(Vector2D pos, Vector2D dir, int speed);
};

#endif //SCENE_HPP
