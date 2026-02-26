//
// Created by navjo on 1/7/2026.
//

//#ifndef GAME_HPP
//#define GAME_HPP
//
//#endif //GAME_HPP

#pragma once //same as above
#include <SDL3/SDL.h>

#include "World.hpp"
#include "manager/SceneManager.hpp"

class Game {
  public:
    Game();
    ~Game();

    SDL_Renderer* renderer = nullptr;

    SceneManager sceneManager;
    static std::function<void(std::string)> onSceneChangeRequest;
    // World world;

    //these are the types SDL uses
    void init(const char* title, int width, int height, bool fullscreen);

    //Game loop functions (handleEvents, update, render)
    void handleEvents(); //checks for input and sys events
    void update(float deltaTime); //handles the game logic and changes to game state
    void render(); //handles the drawing of the current game state to the screen

    //used to free resources
    void destroy();

    bool running() {
        return isRunning;
    }

  private:
    int frameCount = 0;
    bool isRunning = false;

    SDL_Window* window = nullptr;
    SDL_Event event;

    //sdl stores color channels 8 bit unsigned
    //range is 0-255
    Uint8 r,g,b,a;


};
