//
// Created by navjo on 2/25/2026.
//

#include <random>

#include "Scene.hpp"
#include "Game.hpp"
#include "AnimationCallbacks/CarAnim.hpp"
#include "AnimationCallbacks/EnemyAnimCallback.hpp"
#include "AnimationCallbacks/PlayerAnim.hpp"
#include "manager/AssetManager.hpp"

Scene::Scene(SceneType sceneType, const char *sceneName, const char *mapPath, int windowWidth, int windowHeight)
    : name(sceneName), type(sceneType) {

    if (sceneType == SceneType::MainMenu) {
        initMainMenu(windowWidth, windowHeight);
        return;
    }
    initGameplay(mapPath, windowWidth, windowHeight);

}

void Scene::initGameplay(const char *mapPath, int windowWidth, int windowHeight) {
//load our map
    world.getMap().load(mapPath, TextureManager::load("../assets/CP_V1.0.4.png"));

    //create camera
    auto& cam = world.createEntity();
    SDL_FRect camView{};
    camView.w = windowWidth;
    camView.h = windowHeight;
    cam.addComponent<Camera>(camView, world.getMap().width * 16, world.getMap().height * 16);

    //get colliders and object groups
    for (auto& object : world.getMap().regularColliders) {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(object.rect.x, object.rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("wall");
        c.rect.x = object.rect.x;
        c.rect.y = object.rect.y;
        c.rect.w = object.rect.w;
        c.rect.h = object.rect.h;
    }

    for (auto& object : world.getMap().carColliders) {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(object.rect.x, object.rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("car_wall");
        c.rect.x = object.rect.x;
        c.rect.y = object.rect.y;
        c.rect.w = object.rect.w;
        c.rect.h = object.rect.h;
    }

    //Player
    auto& player(world.createEntity());
    SDL_FPoint playerCenter;
    for (auto& object : world.getMap().playerSpawnPoint) {
        auto& playerTransform = player.addComponent<Transform>(Vector2D(object.rect.x, object.rect.y), 0.0f, 1.0f);
        player.addComponent<Velocity>(Vector2D(0.0f, 0.0f), 120.0f);
        Animation anim = AssetManager::getAnimation("player");
        anim.animCallback = PlayerAnim::animCallback;
        player.addComponent<Animation>(anim);
        SDL_Texture* tex = TextureManager::load("../assets/animations/player.png");
        SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
        SDL_FRect playerDst{playerTransform.position.x, playerTransform.position.y, 78, 52};
        auto& playerCollider = player.addComponent<Collider>("player");
        playerCollider.rect.w = playerDst.w;
        playerCollider.rect.h = playerDst.h;
        player.addComponent<Sprite>(tex, playerSrc, playerDst);
        player.addComponent<Health>(Game::gameState.playerHealth, 10);
        player.addComponent<PlayerAnimationState>();
        playerCenter = {playerDst.w/2.0f, playerDst.h/2.0f};
        player.addComponent<Target>(&cam, SDL_FPoint(), playerCenter);
        player.addComponent<CameraFocusTag>(true);
        player.addComponent<KeyboardFocusTag>(true);
        player.addComponent<Interactable>();
        player.addComponent<PlayerTag>();
    }

    //player bullet
    auto& playerBulletSpawner(world.createEntity());
    playerBulletSpawner.addComponent<TimedSpawner>(0.2f, [this, &player, &cam] {
        //for fixing projectile rotation, make sure you have an
        //SDL_FPoint copy variable, so it tracks that point
        //instead of the constantly changing mouse position point
        if (player.getComponent<PlayerAnimationState>().animState != PlayerAnimation::Shooting) return;
        auto& playerTransform = player.getComponent<Transform>();
        auto& playerTarget = player.getComponent<Target>();
        auto playerCenter = playerTarget.startingCenter;
        auto& bullet(world.createDeferredEntity());
        auto& bulletTransform = bullet.addComponent<Transform>(playerTransform);
        bulletTransform.position += Vector2D(playerCenter.x, playerCenter.y);
        Vector2D normalizedDir = {playerTarget.deltaX, playerTarget.deltaY};
        normalizedDir.normalize();
        bullet.addComponent<Velocity>(Vector2D(normalizedDir.x, normalizedDir.y), 600.0f);
        SDL_Texture* bulletTex = TextureManager::load("../assets/ball.png");
        SDL_FRect bulletSrc = {0, 0, 32, 32};
        SDL_FRect bulletDst{bulletTransform.position.x, bulletTransform.position.y, 16, 16};
        bullet.addComponent<Sprite>(bulletTex, bulletSrc, bulletDst);
        auto& bulletCollider = bullet.addComponent<Collider>("player_projectile");
        bulletCollider.rect.w = bulletDst.w;
        bulletCollider.rect.h = bulletDst.h;
        SDL_FPoint bulletCenter {bulletDst.w/2.0f, bulletDst.h/2.0f};
        bullet.addComponent<Target>(&cam, SDL_FPoint(), bulletCenter);
        bullet.addComponent<Damage>(1);
        bullet.addComponent<ProjectileTag>();
    });

    //create car
    auto& car(world.createEntity());
    for (auto& object : world.getMap().carSpawnPoint) {
        auto& carTransform = car.addComponent<Transform>(Vector2D(object.rect.x, object.rect.y), 0.0f, 1.0f);
        car.addComponent<Velocity>(Vector2D(0.0f, 0.0f), 0.0f, 240.0f);
        car.addComponent<Acceleration>(50.0f, SOUTH);
        car.addComponent<Brake>(4.0f);
        Animation carAnim = AssetManager::getAnimation("car");
        carAnim.animCallback = CarAnim::animCallback;
        car.addComponent<Animation>(carAnim);
        SDL_Texture* carTex = TextureManager::load("../assets/animations/car.png");
        SDL_FRect carSrc = carAnim.clips[carAnim.currentClip].frameIndices[0];
        SDL_FRect carDest {carTransform.position.x, carTransform.position.y, 100, 100};
        car.addComponent<Sprite>(carTex, carSrc, carDest);
        auto& carCollider = car.addComponent<Collider>("car");
        carCollider.rect.w = carDest.w;
        carCollider.rect.h = carDest.h;
        car.addComponent<CameraFocusTag>();
        car.addComponent<KeyboardFocusTag>();
        car.addComponent<Interactable>();
        car.addComponent<CarTag>();
    }

    //create enemies
    for (auto& object : world.getMap().enemySpawnPoints) {
        auto& enemy(world.createEntity());
        auto& enemyTransform = enemy.addComponent<Transform>(Vector2D(object.rect.x, object.rect.y), 0.0f, 1.0f);
        enemy.addComponent<Velocity>(Vector2D(0.0f, 0.0f), 120.0f);
        Animation enemyAnim = AssetManager::getAnimation("enemy");
        enemyAnim.animCallback = EnemyAnimCallback::animCallback;
        enemy.addComponent<Animation>(enemyAnim);
        SDL_Texture* enemyTex = TextureManager::load("../assets/animations/enemy.png");
        SDL_FRect enemySrc = enemyAnim.clips[enemyAnim.currentClip].frameIndices[0];
        SDL_FRect enemyDst{enemyTransform.position.x, enemyTransform.position.y, 78, 52};
        auto& enemyCollider = enemy.addComponent<Collider>("enemy");
        enemyCollider.rect.w = enemyDst.w;
        enemyCollider.rect.h = enemyDst.h;
        enemy.addComponent<Sprite>(enemyTex, enemySrc, enemyDst);
        enemy.addComponent<EnemyAnimationState>(EnemyAnimation::Shooting);
        SDL_FPoint enemyCenter {enemyDst.w/2.0f, enemyDst.h/2.0f};
        enemy.addComponent<Target>(&player, playerCenter, enemyCenter);
        enemy.addComponent<Health>(3, 3);
        auto& enemyChildren = enemy.addComponent<Children>();

        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<float> dist(1.0f, 3.0f);
        float randomTime = dist(gen);

        //create enemy bullets
        auto& enemyBulletSpawner(world.createEntity());
        enemyBulletSpawner.addComponent<TimedSpawner>(randomTime, [this, &player, &enemy] {

            if (enemy.isActive() && enemy.getComponent<EnemyAnimationState>().animState != EnemyAnimation::Shooting) return;
            auto& playerTarget = player.getComponent<Target>();
            auto playerCenter = playerTarget.startingCenter;

            auto& enemyTransform = enemy.getComponent<Transform>();
            auto& enemyTarget = enemy.getComponent<Target>();
            auto& enemyCenter = enemyTarget.startingCenter;

            auto& bullet(world.createDeferredEntity());
            auto& bulletTransform = bullet.addComponent<Transform>(enemyTransform);
            bulletTransform.position += Vector2D(enemyCenter.x, enemyCenter.y);
            Vector2D normalizedDir {enemyTarget.deltaX, enemyTarget.deltaY};
            normalizedDir.normalize();
            bullet.addComponent<Velocity>(Vector2D(normalizedDir.x, normalizedDir.y), 600.0f);
            SDL_Texture* bulletTex = TextureManager::load("../assets/ball.png");
            SDL_FRect bulletSrc = {0, 0, 32, 32};
            SDL_FRect bulletDst{bulletTransform.position.x, bulletTransform.position.y, 16, 16};
            bullet.addComponent<Sprite>(bulletTex, bulletSrc, bulletDst);
            auto& bulletCollider = bullet.addComponent<Collider>("enemy_projectile");
            bulletCollider.rect.w = bulletDst.w;
            bulletCollider.rect.h = bulletDst.h;
            SDL_FPoint bulletCenter {bulletDst.w/2.0f, bulletDst.h/2.0f};
            bullet.addComponent<Target>(&player, playerCenter, bulletCenter);
            bullet.addComponent<Damage>(1);
            bullet.addComponent<ProjectileTag>();
        });

        enemyChildren.children.push_back(&enemyBulletSpawner);
    }
    Game::gameState.numEnemies = world.getMap().enemySpawnPoints.size();

    //add acene state
    // auto& state(world.createEntity());
    // state.addComponent<SceneState>();

    createPlayerPosLabel();
    createPlayerWinLabel();
}

Entity &Scene::createSettingsOverlay(int windowWidth, int windowHeight) {
    auto& overlay(world.createEntity());
    SDL_Texture* overlayTex = TextureManager::load("../assets/ui/settings.jpg");
    SDL_FRect overlaySrc {0, 0, windowWidth*0.85f, windowHeight*0.85f};
    SDL_FRect overlayDest {(float)windowWidth/2 - overlaySrc.w/2, (float)windowHeight/2 - overlaySrc.h/2, overlaySrc.w, overlaySrc.h};
    overlay.addComponent<Transform>(Vector2D(overlayDest.x, overlayDest.y), 0.0f, 1.0f);
    overlay.addComponent<Sprite>(overlayTex, overlaySrc, overlayDest, RenderLayer::UI, false);
    createSettingsUIComponents(overlay);
    return overlay;
}

void Scene::initMainMenu(int windowWidth, int windowHeight) {
    //camera
    auto& cam = world.createEntity();
    cam.addComponent<Camera>();

    //menu
    auto& menu(world.createEntity());
    auto& menuTransform = menu.addComponent<Transform>(Vector2D(0, 0), 0.0f, 1.0f);
    SDL_Texture* text = TextureManager::load("../assets/menu.png");
    SDL_FRect menuSrc{0, 0, float(windowWidth), float(windowHeight)};
    SDL_FRect menuDst{menuTransform.position.x, menuTransform.position.y, menuSrc.w, menuSrc.h};
    menu.addComponent<Sprite>(text, menuSrc, menuDst);

    auto& settingsOverlay = createSettingsOverlay(windowWidth, windowHeight);
    createCogButton(windowWidth, windowHeight, settingsOverlay);
}

void Scene::toggleSettingsOverlayVisibility(Entity &overlay) {
    auto& sprite = overlay.getComponent<Sprite>();
    bool newVisibility = !sprite.visible;
    sprite.visible = newVisibility;

    if (overlay.hasComponent<Children>()) {
        auto& c = overlay.getComponent<Children>();

        for (auto& child : c.children) {
            if (child && child->hasComponent<Sprite>()) {
                child->getComponent<Sprite>().visible = newVisibility;
            }

            if (child && child->hasComponent<Collider>()) {
                child->getComponent<Collider>().enabled = newVisibility;
            }
        }
    }
}

Entity &Scene::createCogButton(int windowWidth, int windowHeight, Entity &overlay) {
    auto& cog(world.createEntity());
    auto& cogTransform = cog.addComponent<Transform>(Vector2D((float) windowWidth-50, (float) windowHeight-50), 0.0f, 1.0f);

    SDL_Texture* text = TextureManager::load("../assets/ui/cog.png");
    SDL_FRect cogSrc{0, 0, 32, 32};
    SDL_FRect cogDest{cogTransform.position.x, cogTransform.position.y, cogSrc.w, cogSrc.h};
    cog.addComponent<Sprite>(text, cogSrc, cogDest, RenderLayer::UI);
    cog.addComponent<Collider>("ui", cogDest);
    auto& clickable = cog.addComponent<Clickable>();
    clickable.onPressed = [&cogTransform] {
        cogTransform.scale = 0.5f;
    };
    clickable.onReleased = [&cogTransform, &overlay, this] {
        cogTransform.scale = 1.0f;
        toggleSettingsOverlayVisibility(overlay);
    };
    clickable.onCancel = [&cogTransform] {
        cogTransform.scale = 1.0f;
    };
    return cog;
}

void Scene::createSettingsUIComponents(Entity &overlay) {
    if (!overlay.hasComponent<Children>()) {
        overlay.addComponent<Children>();
    }

    auto& overlayTransform = overlay.getComponent<Transform>();
    auto& overlaySprite = overlay.getComponent<Sprite>();

    float baseX = overlayTransform.position.x;
    float baseY = overlayTransform.position.y;

    auto& closeButton = world.createEntity();
    auto& closeTransform = closeButton.addComponent<Transform>(Vector2D(baseX + overlaySprite.dst.w - 40, baseY + 10), 0.0f, 1.0f);

    SDL_Texture* text = TextureManager::load("../assets/ui/close.png");
    SDL_FRect closeSrc{0, 0, 32, 32};
    SDL_FRect closeDest{closeTransform.position.x, closeTransform.position.y, closeSrc.w, closeSrc.h};
    closeButton.addComponent<Sprite>(text, closeSrc, closeDest, RenderLayer::UI, false);
    closeButton.addComponent<Collider>("ui", closeDest);

    auto& clickable = closeButton.addComponent<Clickable>();
    clickable.onPressed = [&closeTransform] {
      closeTransform.scale = 0.5f;
    };
    clickable.onReleased = [this, &overlay, &closeTransform] {
        closeTransform.scale = 1.0f;
        toggleSettingsOverlayVisibility(overlay);
    };
    clickable.onCancel = [&closeTransform] {
        closeTransform.scale = 1.0f;
    };

    closeButton.addComponent<Parent>(&overlay);
    auto& parentChildren = overlay.getComponent<Children>();
    parentChildren.children.push_back(&closeButton);
}

Entity &Scene::createPlayerPosLabel() {
    auto& playerPositionLabel(world.createEntity());

    Label label = {
        "Test String",
        AssetManager::getFont("arial"),
        {255, 255, 255, 255},
        LabelType::Health,
        "playerHealth"
    };

    TextureManager::loadLabel(label);
    playerPositionLabel.addComponent<Label>(label);

    playerPositionLabel.addComponent<Transform>(Vector2D(10, 10), 0.0f, 1.0f);
    return playerPositionLabel;
}

Entity &Scene::createPlayerWinLabel() {
    auto& playerWinLabel(world.createEntity());

    Label label = {
        "You Win! Press R to restart",
        AssetManager::getFont("arial"),
        {0, 255, 0, 255},
        LabelType::Win,
        "playerWin"
    };
    label.dirty = true;
    label.visible = false;

    TextureManager::loadLabel(label);
    playerWinLabel.addComponent<Label>(label);

    playerWinLabel.addComponent<Transform>(Vector2D(300, 200), 0.0f, 1.0f);
    return playerWinLabel;
}
