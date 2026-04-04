//
// Created by navjo on 2/25/2026.
//

#include "Scene.hpp"

#include <iostream>

#include "Game.hpp"
#include "AnimationCallbacks/CarAnim.hpp"
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
    world.getMap().load(mapPath, TextureManager::load("../assets/spritesheet.png"));

    //get colliders
    for (auto &collider : world.getMap().colliders) {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("no_wall");
        c.rect.x = collider.rect.x;
        c.rect.y = collider.rect.y;
        c.rect.w = collider.rect.w;
        c.rect.h = collider.rect.h;

        SDL_Texture* tex = TextureManager::load("../assets/spritesheet.png");
        SDL_FRect colSrc {2, 36, 32, 32};
        SDL_FRect colDst {c.rect.x, c.rect.y, c.rect.w, c.rect.h};

        e.addComponent<Sprite>(tex, colSrc, colDst);
    }

    //add coins to designated points on the map
    for (auto &collider : world.getMap().spawnPoints) {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("item");

        c.rect.x = collider.rect.x;
        c.rect.y = collider.rect.y;
        c.rect.w = collider.rect.w;
        c.rect.h = collider.rect.h;

        SDL_Texture* itemTex = TextureManager::load("../assets/coin.png");
        SDL_FRect colSrc {0, 0, 32, 32};
        SDL_FRect colDst {c.rect.x, c.rect.y, 32, 32};

        e.addComponent<Sprite>(itemTex, colSrc, colDst);
    }

    //create camera
    auto& cam = world.createEntity();
    SDL_FRect camView{};
    camView.w = windowWidth;
    camView.h = windowHeight;
    cam.addComponent<Camera>(camView, world.getMap().width * 32, world.getMap().height * 32);

    //Player
    auto& player(world.createEntity());
    auto& playerTransform = player.addComponent<Transform>(Vector2D(0, 0), 0.0f, 1.0f);
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
    player.addComponent<Health>(Game::gameState.playerHealth);
    player.addComponent<PlayerAnimationState>();
    SDL_FPoint playerCenter {playerDst.w/2.0f, playerDst.h/2.0f};
    player.addComponent<Target>(&cam, SDL_FPoint(), playerCenter);
    player.addComponent<CameraFocusTag>(true);
    player.addComponent<KeyboardFocusTag>(true);
    player.addComponent<Interactable>();
    player.addComponent<PlayerTag>();

    //player bullet
    auto& b(world.createEntity());
    b.addComponent<TimedSpawner>(0.2f, [this, &player, &cam] {
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
        bullet.addComponent<ProjectileTag>();
    });

    //create car
    auto& car(world.createEntity());
    auto& carTransform = car.addComponent<Transform>(Vector2D(100, 100), 0.0f, 1.0f);
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

    auto& state(world.createEntity());
    state.addComponent<SceneState>();
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






