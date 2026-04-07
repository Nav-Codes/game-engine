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
#include "SDL3_ttf/SDL_ttf.h"

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

enum class PlayerAnimation {
    Idle,
    Walking,
    Shooting
};

struct PlayerAnimationState {
    PlayerAnimation animState{};
};

enum class EnemyAnimation {
    Idle,
    Walking,
    Shooting
};

struct EnemyAnimationState {
    EnemyAnimation animState{};
};

struct Target {
    Entity* target = nullptr;
    SDL_FPoint targetCenter{};
    SDL_FPoint startingCenter{};
    float deltaX{};
    float deltaY{};
};

enum class RenderLayer {
    World,
    UI,
    None
};

struct Sprite {
    SDL_Texture* texture = nullptr;
    SDL_FRect src{};
    SDL_FRect dst{};
    RenderLayer renderLayer = RenderLayer::World;
    bool visible = true;
};

struct Collider {
    string tag;
    SDL_FRect rect{}; //position and size of collider
    bool enabled = true;
};

struct Interactable {
    bool interactable{};
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
    int maxHealth{};
};

struct Damage {
    int damage{};
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

enum class LabelType {PlayerPosition,Damage,Health};

struct Label {
    string text;
    TTF_Font* font = nullptr;
    SDL_Color color{};
    LabelType type = LabelType::PlayerPosition;
    string textureCacheKey{};
    SDL_Texture* texture = nullptr;
    SDL_FRect dst{};
    bool visible = true;
    bool dirty = false;
};

struct PlayerTag{};

struct ProjectileTag{};

struct CarTag{};

struct CameraFocusTag {
    bool active{};
};

struct KeyboardFocusTag {
    bool active{};
};

#endif //COMPONENT_HPP
