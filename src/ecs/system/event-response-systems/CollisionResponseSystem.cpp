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
        onPlayerCollision(collision, "enemy_projectile", world);

        onEnemyCollision(collision, "player_projectile", world);

        onCarCollision(collision, "car_wall", world);

        onProjectileCollision(collision, "car", world);
        onProjectileCollision(collision, "wall", world);
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

    else if (tag == "enemy_projectile") {
        if (e.state != CollisionState::Enter) return;

        if (player->hasComponent<Health>() && other->hasComponent<Damage>()) {
            auto& playerHealth = player->getComponent<Health>();
            auto& damage = other->getComponent<Damage>();

            playerHealth.currentHealth -= damage.damage;

            if (playerHealth.currentHealth <= 0) {
                Game::onSceneChangeRequest("level1");
            }
        }
    }
}

void CollisionResponseSystem::onCarCollision(const CollisionEvent &e, const char *otherTag, World &world) {
    Entity* car = nullptr;
    Entity* other = nullptr;

    if (!getEntities(e, otherTag, car, other, "car")) return;

    string otherEntityTag = string(otherTag);

    if (otherEntityTag == "car_wall") {
        if (e.state == CollisionState::Enter) {
            auto& acc = car->getComponent<Acceleration>();
            acc.isAccelerating = false;
        }
        else if (e.state == CollisionState::Stay) {
            auto& t = car->getComponent<Transform>();
            t.position = t.oldPosition;
            auto& vel = car->getComponent<Velocity>();
            vel.speed = 0;
        }
    }

    if (otherEntityTag == "enemy_projectile" || otherEntityTag == "player_projectile") {
        if (e.state != CollisionState::Enter) return;

        other->destroy();
    }
}

void CollisionResponseSystem::onEnemyCollision(const CollisionEvent &e, const char *otherTag, World &world) {
    Entity* enemy = nullptr;
    Entity* other = nullptr;

    if (!getEntities(e, otherTag, enemy, other, "enemy")) return;

    string otherEntityTag = string(otherTag);

    if (otherEntityTag == "player_projectile") {
        if (e.state != CollisionState::Enter) return;

        if (enemy->hasComponent<Health>() && other->hasComponent<Damage>()) {
            auto& enemyHealth = enemy->getComponent<Health>();
            auto& damage = other->getComponent<Damage>();

            enemyHealth.currentHealth -= damage.damage;

            if (enemyHealth.currentHealth == 0 && enemy->hasComponent<Children>()) {
                auto& enemyChildren = enemy->getComponent<Children>();
                for (auto& childEntity : enemyChildren.children) {
                    childEntity->destroy();
                }
                enemy->destroy();
            }
        }
    }
}

void CollisionResponseSystem::onProjectileCollision(const CollisionEvent &e, const char *otherTag, World &world) {
    Entity* projectile = nullptr;
    Entity* other = nullptr;

    if (!getEntities(e, otherTag, projectile, other, "player_projectile")) {
        if (!getEntities(e, otherTag, projectile, other, "enemy_projectile")) return;
    }

    string otherEntityTag = string(otherTag);

    if (otherEntityTag == "car" || otherEntityTag == "wall") {
        if (e.state != CollisionState::Enter) return;

        projectile->destroy();
    }
}
