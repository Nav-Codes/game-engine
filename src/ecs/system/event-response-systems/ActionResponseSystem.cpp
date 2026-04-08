//
// Created by navjo on 4/5/2026.
//

#include "ActionResponseSystem.hpp"
#include "World.hpp"
#include "manager/AssetManager.hpp"

ActionResponseSystem::ActionResponseSystem(World &world) {
    world.getEventManager().subscribe(
    [this, &world](const BaseEvent& e) {

        if (e.type != EventType::PlayerAction) return;
        const auto& playerAction = static_cast<const PlayerActionEvent&>(e);

        onPlayerAction(playerAction, world);
    });

    world.getEventManager().subscribe(
    [this, &world](const BaseEvent& e) {

        if (e.type != EventType::CarAction) return;
        const auto& carAction = static_cast<const CarActionEvent&>(e);

        onCarAction(carAction);
    });
}

void ActionResponseSystem::onCarAction(const CarActionEvent &e) {
    auto& car = e.car;
    if (!car->hasComponent<Velocity>() || !car->hasComponent<Acceleration>() || !car->hasComponent<Brake>())
        return;
    auto& v = car->getComponent<Velocity>();
    auto& a = car->getComponent<Acceleration>();
    auto& b = car->getComponent<Brake>();

    if (e.action == CarAction::Accelerate) {
        if ((a.direction == e.oppositeDir && v.speed > ZERO_EPSILON) || (a.direction == e.dir && v.speed < -ZERO_EPSILON)) {
            a.isAccelerating = false;
            b.isBraking = true;
        }
        else if (a.direction == e.oppositeDir && v.speed == 0.0f) {
            a.isAccelerating = true;
            b.isBraking = false;
            v.speed = -JUMP_START;
        }
        else if (a.direction == e.dir || a.direction == e.turnDir1 || a.direction == e.turnDir2) {
            if (v.speed == 0.0f) {
                v.speed = JUMP_START;
            }
            a.direction = v.speed > 0.0f ? e.dir : e.oppositeDir;
            a.isAccelerating = true;
            b.isBraking = false;
        }
    }

    else if (e.action == CarAction::Decelerate) {
        a.isAccelerating = false;
        b.isBraking = false;
    }
}

void ActionResponseSystem::onPlayerAction(const PlayerActionEvent &e, World& world) {
    // player and car interaction
    if (e.action == PlayerAction::CarInteract) {
        //things to do to player
        if (e.player->hasComponent<PlayerTag>()) {
            auto& player = e.player;
            auto& playerSprite = player->getComponent<Sprite>();
            playerSprite.visible = !playerSprite.visible;
            auto& playerKeyboardTag = player->getComponent<KeyboardFocusTag>();
            playerKeyboardTag.active = !playerKeyboardTag.active;
            auto& playerCameraTag = player->getComponent<CameraFocusTag>();
            playerCameraTag.active = !playerCameraTag.active;
            auto& vel = player->getComponent<Velocity>();
            vel.direction.x = vel.direction.y = 0;
        }

        //things to do for car
        else if (e.player->hasComponent<CarTag>()) {
            auto& car = e.player;
            auto& carKeyboardTag = car->getComponent<KeyboardFocusTag>();
            carKeyboardTag.active = !carKeyboardTag.active;
            auto& carCameraTag = car->getComponent<CameraFocusTag>();
            carCameraTag.active = !carCameraTag.active;
        }
    }
    else if (e.action == PlayerAction::Win) {
        auto& playerWinLabel(world.createEntity());

        Label label = {
            "You Win! Press R to restart",
            AssetManager::getFont("arial"),
            {0, 255, 0, 255},
            LabelType::Win,
            "playerWin"
        };
        label.dirty = true;
        label.visible = true;

        TextureManager::loadLabel(label);
        playerWinLabel.addComponent<Label>(label);

        playerWinLabel.addComponent<Transform>(Vector2D(300, 200), 0.0f, 1.0f);
    }
}


