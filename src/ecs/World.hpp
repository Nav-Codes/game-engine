//
// Created by navjo on 1/21/2026.
//

#ifndef WORLD_HPP
#define WORLD_HPP
#include <memory>
#include <vector>

#include "AnimationSystem.hpp"
#include "CameraSystem.hpp"
#include "CollisionSystem.hpp"
#include "DestructionSystem.hpp"
#include "Entity.hpp"
#include "system/event-response-systems/MouseUIResponseSystem.hpp"
#include "HUDSystem.hpp"
#include "KeyboardInputSystem.hpp"
#include "MainMenuSystem.hpp"
#include "Map.hpp"
#include "MouseInputSystem.hpp"
#include "Movement.hpp"
#include "RenderSystem.hpp"
#include "SpawnTimerSystem.hpp"
#include "PhysicsSystem.hpp"
#include "PreRenderSystem.hpp"
#include "RotationSystem.hpp"
#include "TargetingSystem.hpp"
#include "UIRenderSystem.hpp"
#include "event-response-systems/ActionResponseSystem.hpp"
#include "event-response-systems/CollisionResponseSystem.hpp"
#include "event/EventManager.hpp"
#include "scene/SceneType.hpp"

using namespace std;

class World {
    Map map;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Entity>> deferredEntities;

    PhysicsSystem physicsSystem;
    MovementSystem movementSystem;
    RenderSystem renderSystem;
    KeyboardInputSystem keyboardInputSystem;
    CollisionSystem collisionSystem;
    AnimationSystem animationSystem;
    CameraSystem cameraSystem;
    EventManager eventManager;
    SpawnTimerSystem spawnTimerSystem;
    DestructionSystem destructionSystem;
    MouseUIResponseSystem eventResponseSystem{*this};
    ActionResponseSystem actionResponseSystem{*this};
    CollisionResponseSystem collisionResponseSystem{*this};
    MainMenuSystem mainMenuSystem;
    UIRenderSystem uiRenderSystem;
    MouseInputSystem mouseInputSystem;
    HUDSystem hudSystem;
    PreRenderSystem preRenderSystem;
    RotationSystem rotationSystem;
    TargetingSystem targetingSystem;

public:
    World() = default;
    void update(float dt, const SDL_Event& event, SceneType sceneType) {

        if (sceneType == SceneType::MainMenu) {
            mainMenuSystem.update(event);
        }
        else {
            keyboardInputSystem.update(entities, event, *this);
            physicsSystem.update(entities, dt);
            movementSystem.update(entities, dt);
            collisionSystem.update(*this);
            animationSystem.update(entities, dt);
            targetingSystem.update(entities);
            rotationSystem.update(entities);
            cameraSystem.update(entities);
            spawnTimerSystem.update(entities, dt);
            destructionSystem.update(entities);
            hudSystem.update(entities);
        }

        mouseInputSystem.update(*this, event);
        preRenderSystem.update(entities);

        synchronizeEntities();
        cleanup();
    }

    void render() {
        for (auto& entity : entities) {
            if (entity->hasComponent<Camera>()) {
                map.draw(entity->getComponent<Camera>());
                break;
            }
        }

        renderSystem.render(entities);
        uiRenderSystem.render(entities);
    }

    Entity& createEntity() {
        //use emplace instead of push so we dont create a copy
        entities.emplace_back(std::make_unique<Entity>());
        return *entities.back();
    }

    Entity& createDeferredEntity() {
        deferredEntities.emplace_back(std::make_unique<Entity>());
        return *deferredEntities.back();
    }

    std::vector<std::unique_ptr<Entity>>& getEntities() {
        return entities;
    }

    void cleanup() {
        //use a lambda predicate to remove all inactive entities
        std::erase_if
        (
            entities,
            [](std::unique_ptr<Entity>& e) {
                return !e->isActive();
            }
        );

    }

    void synchronizeEntities() {
        if (!deferredEntities.empty()) {
            //push back all deferred entities to the entities vector
            //using move so we dont create a copy
            std::move (
                deferredEntities.begin(),
                deferredEntities.end(),
                std::back_inserter(entities)
                );
            //clearing the creation buffer
            deferredEntities.clear();
        }
    }

    EventManager& getEventManager() {
        return eventManager;
    }

    Map& getMap() {
        return map;
    }
};

#endif //WORLD_HPP
