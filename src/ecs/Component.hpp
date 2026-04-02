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
    float maxSpeed{};
};

struct Acceleration {
    float accelerationFactor = 1.0f;
    //controls which direction the object is facing
    //allows object to move south while facing north as if it is reversing
    Direction direction{};
    bool isAccelerating{};
};

struct Brake {
    float brakeForce = 1.0f;
    bool isBraking{};
};

enum class PlayerState {
    Idle,
    Walking,
    Shooting
};

struct PlayerActionState {
    PlayerState playerState{};
    //ensures player is idle only when player is not holding down any of these keys
    bool W{}, A{}, S{}, D{};
};

struct Rotator {
    Transform* target = nullptr;
};

struct Targeting {
    //could have rise/run thingy here and have player rotation system access this
    //rise and run can be calculated in another targeting system
    Entity& start;
    Entity& target;
    float rise;
    float run;
};

enum class RenderLayer {
    World,
    UI
};

struct Sprite {
    SDL_Texture* texture = nullptr;
    SDL_FRect src{};
    SDL_FRect dst{};
    RenderLayer renderLayer = RenderLayer::World;
    bool visible = true;
    SDL_FPoint center{};
};

struct Collider {
    string tag;
    SDL_FRect rect{}; //position and size of collider
    bool enabled = true;
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

struct Health {
    int currentHealth{};
};

struct Clickable {
    std::function<void()> onPressed{};
    std::function<void()> onReleased{};
    std::function<void()> onCancel{};
    bool pressed = true;
};

struct Parent {
    Entity* parent = nullptr;
};

struct Children {
    std::vector<Entity*> children{};
};

struct PlayerTag{};

struct ProjectileTag{};

struct CarTag{};

#endif //COMPONENT_HPP
