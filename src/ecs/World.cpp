//
// Created by navjo on 2/4/2026.
//
#include "World.hpp"

#include <iostream>

#include "Game.hpp"

// void SecondObserver(const CollisionEvent& collision) {
//     std::cout << "A collision occurred between " << collision.entityA->getComponent<Collider>().tag << " and " << collision.entityB->getComponent<Collider>().tag << std::endl;
// }

World::World() {
    //subscribe to the collision events
    eventManager.subscribe([this](const CollisionEvent& collision) {
        Entity* sceneStateEntity = nullptr;

        //find scene state
        for (auto& e : entities) {
            if (e->hasComponent<SceneState>()) {
                sceneStateEntity = e.get();
                break;
            }
        }

        if (!sceneStateEntity) return;

        if (collision.entityA == nullptr || collision.entityB == nullptr) return;

        if (!(collision.entityA->hasComponent<Collider>() && collision.entityB->hasComponent<Collider>())) return;

        auto& colliderA = collision.entityA->getComponent<Collider>();
        auto& colliderB = collision.entityB->getComponent<Collider>();

        Entity* player = nullptr;
        Entity* item = nullptr;
        Entity* wall = nullptr;
        Entity* projectile = nullptr;


        if (colliderA.tag == "player" && colliderB.tag == "item") {
            player = collision.entityA;
            item = collision.entityB;
        } else if (colliderA.tag == "item" && colliderB.tag == "player") {
            player = collision.entityB;
            item = collision.entityA;
        }

        if (player && item) {
            item->destroy();
            //scene state
            auto& sceneState = sceneStateEntity->getComponent<SceneState>();
            sceneState.coinsCollected++;

            if (sceneState.coinsCollected > 1) {
                Game::onSceneChangeRequest("level2");
            }
        }

        //player vs wall
        if (colliderA.tag == "player" && colliderB.tag == "wall") {
            player = collision.entityA;
            wall = collision.entityB;
        } else if (colliderA.tag == "wall" && colliderB.tag == "player") {
            player = collision.entityB;
            wall = collision.entityA;
        }

        if (player && wall) {
            //stop player
            auto& t = player->getComponent<Transform>();
            t.position = t.oldPosition;
        }


        //player vs projectile
        if (colliderA.tag == "player" && colliderB.tag == "projectile") {
            player = collision.entityA;
            projectile = collision.entityB;
        } else if (colliderA.tag == "projectile" && colliderB.tag == "player") {
            player = collision.entityB;
            projectile = collision.entityA;
        }

        if (player && projectile) {
            player->destroy();
            //change scenes defer
            Game::onSceneChangeRequest("gameover");
        }
    });

    // eventManager.subscribe<CollisionEvent>(SecondObserver);
}
