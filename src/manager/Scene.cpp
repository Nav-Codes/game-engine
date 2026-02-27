//
// Created by navjo on 2/25/2026.
//

#include "Scene.hpp"

#include "AssetManager.hpp"

Scene::Scene(const char *sceneName, const char *mapPath, int windowWidth, int windowHeight) : name(sceneName) {
//load our map
    world.getMap().load(mapPath, TextureManager::load("../assets/spritesheet.png"));

    for (auto &collider : world.getMap().colliders) {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto& c = e.addComponent<Collider>("no_wall");
        c.rect.x = collider.rect.x;
        c.rect.y = collider.rect.y;
        c.rect.w = collider.rect.w;
        c.rect.h = collider.rect.h;

        SDL_Texture* tex = TextureManager::load("../assets/spritesheet.png");
        SDL_FRect colSrc {0, 32, 32, 32};
        SDL_FRect colDst {c.rect.x, c.rect.y, c.rect.w, c.rect.h};

        e.addComponent<Sprite>(tex, colSrc, colDst);
    }

    //add entities
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

    auto& cam = world.createEntity();
    SDL_FRect camView{};
    camView.w = windowWidth;
    camView.h = windowHeight;
    cam.addComponent<Camera>(camView, world.getMap().width * 32, world.getMap().height * 32);

    //Player
    auto& player(world.createEntity());
    auto& playerTransform = player.addComponent<Transform>(Vector2D(0, 0), 0.0f, 1.0f);
    player.addComponent<Velocity>(Vector2D(0.0f, 0.0f), 120.0f);

    Animation anim = AssetManager::getAnimation("player_anim");
    player.addComponent<Animation>(anim);

    SDL_Texture* tex = TextureManager::load("../assets/animations/bird_anim.png");

    // SDL_FRect playerSrc{0, 0, 32, 44};
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDst{playerTransform.position.x, playerTransform.position.y, 64, 64};

    auto& playerCollider = player.addComponent<Collider>("player");
    playerCollider.rect.w = playerDst.w;
    playerCollider.rect.h = playerDst.h;

    player.addComponent<Sprite>(tex, playerSrc, playerDst);
    player.addComponent<PlayerTag>();

    //create projectile
    auto& spawner(world.createEntity());
    Transform t = spawner.addComponent<Transform>(Vector2D(windowWidth/2, windowHeight-5), 0.0f, 1.0f);
    spawner.addComponent<TimedSpawner>(2.0f, [this, t] {
        //create our projectile
        auto& e(world.createDeferredEntity());
        e.addComponent<Transform>(Vector2D(t.position.x, t.position.y), 0.0f, 1.0f);
        e.addComponent<Velocity>(Vector2D(0,-1), 100.0f);

        auto& anim = AssetManager::getAnimation("enemy");
        e.addComponent<Animation>(anim);

        SDL_Texture* tex = TextureManager::load("../assets/animations/enemy_anim.png");
        SDL_FRect src {0, 0, 32, 32};
        SDL_FRect dest {t.position.x, t.position.y, 32, 32};

        e.addComponent<Sprite>(tex, src, dest);

        auto& c = e.addComponent<Collider>("projectile");
        c.rect.w = dest.w;
        c.rect.h = dest.h;

        e.addComponent<ProjectileTag>();
    });

    auto& state(world.createEntity());
    state.addComponent<SceneState>();
}

void Scene::createProjectile(Vector2D pos, Vector2D dir, int speed) {

}

