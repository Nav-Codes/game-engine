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
            onCollision(collision, "projectile", world);
        });

    world.getEventManager().subscribe(
    [this, &world](const BaseEvent& e) {

        if (e.type != EventType::PlayerAction) return;
        const auto& playerAction = static_cast<const PlayerActionEvent&>(e);

        //TODO onPlayerAction

    });
}

void EventResponseSystem::onCollision(const CollisionEvent &e, const char *otherTag, World &world) {
    Entity* player = nullptr;
    Entity* other = nullptr;

    if (!getCollisionEntities(e, otherTag, player, other)) return;

    if (std::string(otherTag) == "item") {
        other->destroy();
        //scene state

        for (auto& entity : world.getEntities()) {
            if (!entity->hasComponent<SceneState>()) continue;

            auto& sceneState = entity->getComponent<SceneState>();
            sceneState.coinsCollected++;
            if (sceneState.coinsCollected > 1) {
                Game::onSceneChangeRequest("level2");
            }
        }
    } else if (std::string(otherTag) == "wall") {
        auto& t = player->getComponent<Transform>();
        t.position = t.oldPosition;
    } else if (std::string(otherTag) == "projectile") {
        player->destroy();
        //change scenes defer
        Game::onSceneChangeRequest("gameover");
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

void EventResponseSystem::onPlayerAction(const PlayerActionEvent &e, const std::function<void(Entity *player, PlayerAction action)> &callback) {

}



