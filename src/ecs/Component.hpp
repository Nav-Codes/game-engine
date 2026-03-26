//
// Created by navjo on 1/21/2026.
//

#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <functional>
#include <SDL3/SDL_render.h>
#include <string>
#include <unordered_map>

#include "AnimationClip.hpp"
#include "Direction.hpp"
#include "Entity.hpp"
#include "utils/Vector2D.hpp"

using namespace std;

struct Transform {
    Vector2D position{};
    float rotation{};
    float scale{};
    Vector2D oldPosition{};
};

//direction and speed
struct Velocity {
    Vector2D direction{};
    float speed{};
    // float accelerationFactor{}; // use this if its >= 0
    float maxSpeed{};
    // Direction facingDirection{};
};

struct Acceleration {
    float accelerationFactor{}; // use this if its >= 0
    //controls which direction the object is facing
    //allows object to move south while facing north as if it is reversing
    Direction direction{};
    bool isAccelerating{};
    bool isBraking{};
};

struct Sprite {
    SDL_Texture* texture  = nullptr;
    SDL_FRect src{};
    SDL_FRect dst{};
};

struct Collider {
    string tag;
    SDL_FRect rect{}; //position and size of collider
};

struct Animation {
    unordered_map<string, AnimationClip> clips{};
    string currentClip{};
    float time{}; //time is accumulated for curr frame
    int currentFrame{}; //index of curr frame
    float speed = 0.1f; //time per frame
    string oldClip{};
    function<string(Entity&)> animCallback = nullptr; // returns newClip so animationSystem can handle rest of animation thingies
};

struct Camera {
    SDL_FRect view;
    float worldWidth;
    float worldHeight;
};

struct TimedSpawner {
    float spawnInterval{};
    std::function<void()> spawnCallback{};
    float timer{};
};

//our game state, might have multiple scenes
struct SceneState {
    int coinsCollected = 0;
};

struct PlayerTag{};

struct ProjectileTag{};

struct CarTag{};

#endif //COMPONENT_HPP
