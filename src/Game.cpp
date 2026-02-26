//
// Created by navjo on 1/7/2026.
//
#include "Game.hpp"
#include "Map.hpp"

#include <iostream>
#include <ostream>
#include <random>

// #include "AssetManager.hpp"
#include "manager/AssetManager.hpp"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(0, 255);
std::function<void(std::string)> Game::onSceneChangeRequest;
// GameObject *player = nullptr;

Game::Game() {

}

Game::~Game() {
    destroy();
}

void Game::init(const char *title, int width, int height, bool fullscreen) {
    int flags = 0;

    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    //init SDL library
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == 1) {
        std::cout << "Subsystem initialized..." << std::endl;
        window = SDL_CreateWindow(title, width, height, flags);
        if (window) {
            std::cout << "Window Created..." << std::endl;
        }

        //windows will be direct3d
        //mac will be metal or older macs is opengl
        renderer = SDL_CreateRenderer(window, nullptr);

        if (renderer) {
            std::cout << "Renderer created..." << std::endl;
        } else {
            std::cout << "Renderer could not be created." << std::endl;
        }

        isRunning = true;
    } else {
        isRunning = false;
    }

    //load assets
    AssetManager::loadAnimation("player_anim", "../assets/animations/bird_anim.xml");
    AssetManager::loadAnimation("enemy", "../assets/animations/enemy_animations.xml");

    //load scenes
    sceneManager.loadScene("level1", "../assets/map.tmx", width, height);
    sceneManager.loadScene("level2", "../assets/map2.tmx", width, height);

    //start level 1
    sceneManager.changeSceneDeferred("level1");

    //resolve scene callbakc
    onSceneChangeRequest = [this](string sceneName) {
        if (sceneManager.currentScene->getName() == "level2" && sceneName=="level2") {
            cout << "u win" << endl;
            isRunning = false;
            return;
        }

        if (sceneName == "gameover") {
            cout << "u died" << endl;
            isRunning = false;
            return;
        }

        sceneManager.changeSceneDeferred(sceneName);
    };

}

void Game::handleEvents() {
    //check for next event, if an event is available, it will remove from q and store in event variable
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}

void Game::update(float deltaTime) {
    // world.update(deltaTime, event);
    sceneManager.update(deltaTime, event);
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //every frame, renderer is cleared
    SDL_RenderClear(renderer);

    //all your drawing would go here
    // player->draw();
    // world.render();

    //displays everything that was just drawn
    //draws it in mem first to back buffer

    sceneManager.render();

    //swaps back buffer to screen
    SDL_RenderPresent(renderer);
}

void Game::destroy() {
    TextureManager::clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game Destroyed!" << std::endl;
}
