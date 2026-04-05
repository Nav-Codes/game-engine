//
// Created by navjo on 3/25/2026.
//
#include "EventResponseSystem.hpp"

#include "Game.hpp"
#include "World.hpp"

EventResponseSystem::EventResponseSystem(World &world) {
    //subscriptions
    world.getEventManager().subscribe(
        [this, &world](const BaseEvent& e) {

            if (e.type != EventType::Collision) return;
            const auto& collision = static_cast<const CollisionEvent&>(e);

            onCollision(collision, "item", world);
            onCollision(collision, "wall", world);
            onCollision(collision, "player_projectile", world);
            onCollision(collision, "enemy_projectile", world);
            onCollision(collision, "car", world);
        });

    world.getEventManager().subscribe([this, &world](const BaseEvent& e) {
        if (e.type != EventType::MouseInteraction) return;
        const auto& mouseInteractionEvent = static_cast<const MouseInteractionEvent&>(e);
        onMouseInteraction(mouseInteractionEvent);
    });
}

void EventResponseSystem::onMouseInteraction(const MouseInteractionEvent &e) {
    if (!e.entity->hasComponent<Clickable>()) return;

    auto& clickable = e.entity->getComponent<Clickable>();

    switch (e.state) {
        case MouseInteractionState::Pressed :
            clickable.onPressed();
            break;
        case MouseInteractionState::Released :
            clickable.onReleased();
            break;
        case MouseInteractionState::Cancel:
            clickable.onCancel();
            break;
        default : break;
    }
}

void EventResponseSystem::onCollision(const CollisionEvent &e, const char *otherTag, World &world) {
    Entity* player = nullptr;
    Entity* other = nullptr;

    if (!getCollisionEntities(e, otherTag, player, other)) return;

    //item collision
    if (std::string(otherTag) == "item") {
        //scene state
        if (e.state != CollisionState::Enter) return;

        for (auto& entity : world.getEntities()) {
            if (!entity->hasComponent<SceneState>()) continue;

            auto& sceneState = entity->getComponent<SceneState>();
            sceneState.coinsCollected++;
            if (sceneState.coinsCollected > 1) {
                Game::onSceneChangeRequest("level2");
            }
        }
        other->destroy();
    }

    //wall collision
    else if (std::string(otherTag) == "wall") {

        if (e.state != CollisionState::Stay) return;

        auto& t = player->getComponent<Transform>();
        t.position = t.oldPosition;
    }

    //projectile collision
    else if (std::string(otherTag) == "enemy_projectile") {
        if (e.state != CollisionState::Enter) return;

        auto& health = player->getComponent<Health>();
        health.currentHealth--;
        Game::gameState.playerHealth = health.currentHealth;

        cout << "Health: " << health.currentHealth << endl;

        if (health.currentHealth <= 0) {
            player->destroy();
            //change scenes defer
            Game::onSceneChangeRequest("gameover");
        }
    }

    //car collision
    else if (std::string(otherTag) == "car") {
        if (e.state == CollisionState::Stay) {
            //when player gets out of the car
            if (other->getComponent<CameraFocusTag>().active && other->getComponent<KeyboardFocusTag>().active) {
                player->getComponent<Transform>().position = other->getComponent<Transform>().position;
            }
        }
        if (e.state == CollisionState::Enter) {
            if (player->hasComponent<Interactable>() && other->hasComponent<Interactable>()) {
                player->getComponent<Interactable>().interactable = true;
                other->getComponent<Interactable>().interactable = true;
            }

        } else if (e.state == CollisionState::Exit) {
            if (player->hasComponent<Interactable>() && other->hasComponent<Interactable>()) {
                player->getComponent<Interactable>().interactable = false;
                other->getComponent<Interactable>().interactable = false;
                other->getComponent<Acceleration>().isAccelerating = false;
            }
        }
    }
}

bool EventResponseSystem::getCollisionEntities(const CollisionEvent &e, const char *otherTag, Entity *&player, Entity *&other) {
    if (e.entityA == nullptr || e.entityB == nullptr) return false;

    if (!(e.entityA->hasComponent<Collider>() && e.entityB->hasComponent<Collider>())) return false;

    auto& colliderA = e.entityA->getComponent<Collider>();
    auto& colliderB = e.entityB->getComponent<Collider>();

    if (colliderA.tag == "player" && colliderB.tag == otherTag) {
        player = e.entityA;
        other = e.entityB;
    } else if (colliderA.tag == otherTag && colliderB.tag == "player") {
        player = e.entityB;
        other = e.entityA;
    }

    return player && other;
}




