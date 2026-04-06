//
// Created by navjo on 4/5/2026.
//

#include "CollisionResponseSystem.hpp"
#include "World.hpp"
#include "Game.hpp"

CollisionResponseSystem::CollisionResponseSystem(World &world) {
    world.getEventManager().subscribe(
    [this, &world](const BaseEvent& e) {

        if (e.type != EventType::Collision) return;
        const auto& collision = static_cast<const CollisionEvent&>(e);

        onPlayerCollision(collision, "wall", world);
        onPlayerCollision(collision, "car", world);

        onCarCollision(collision, "car_wall", world);
    });

}

bool CollisionResponseSystem::getEntities(const CollisionEvent &e, const char *otherTag, Entity *&focus, Entity *&other, std::string focusedEntity) {
    if (e.entityA == nullptr || e.entityB == nullptr) return false;

    if (!(e.entityA->hasComponent<Collider>() && e.entityB->hasComponent<Collider>())) return false;

    auto& colliderA = e.entityA->getComponent<Collider>();
    auto& colliderB = e.entityB->getComponent<Collider>();

    if (colliderA.tag == focusedEntity && colliderB.tag == otherTag) {
        focus = e.entityA;
        other = e.entityB;
    } else if (colliderA.tag == otherTag && colliderB.tag == focusedEntity) {
        focus = e.entityB;
        other = e.entityA;
    }

    return focus && other;
}

void CollisionResponseSystem::onPlayerCollision(const CollisionEvent &e, const char *otherTag, World &world) {
    Entity* player = nullptr;
    Entity* other = nullptr;

    if (!getEntities(e, otherTag, player, other, "player")) return;

    string tag = string(otherTag);

    if (tag == "wall") {
        if (e.state != CollisionState::Stay) return;

        auto& t = player->getComponent<Transform>();
        t.position = t.oldPosition;
    }

    else if (tag == "car") {
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
        }
        else if (e.state == CollisionState::Exit) {
            if (player->hasComponent<Interactable>() && other->hasComponent<Interactable>()) {
                player->getComponent<Interactable>().interactable = false;
                other->getComponent<Interactable>().interactable = false;
                other->getComponent<Acceleration>().isAccelerating = false;
            }
        }
    }
}

void CollisionResponseSystem::onCarCollision(const CollisionEvent &e, const char *otherTag, World &world) {
    Entity* car = nullptr;
    Entity* other = nullptr;

    if (!getEntities(e, otherTag, car, other, "car")) return;

    string tag = string(otherTag);

    if (tag == "car_wall") {
        if (e.state != CollisionState::Stay) return;

        auto& t = car->getComponent<Transform>();
        t.position = t.oldPosition;
    }
}


// void CollisionResponseSystem::onCollision(const CollisionEvent &e, const char *otherTag, World &world) {
//
//     //projectile collision
//     else if (std::string(otherTag) == "enemy_projectile") {
//         if (e.state != CollisionState::Enter) return;
//
//         auto& health = player->getComponent<Health>();
//         health.currentHealth--;
//         Game::gameState.playerHealth = health.currentHealth;
//
//         cout << "Health: " << health.currentHealth << endl;
//
//         if (health.currentHealth <= 0) {
//             player->destroy();
//             //change scenes defer
//             Game::onSceneChangeRequest("gameover");
//         }
//     }
// }