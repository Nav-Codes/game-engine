#include <iostream>
#include "Game.hpp"

Game *game = nullptr;

int main() {
    const int FPS = 60;
    const int desiredFrameTime = 1000 / FPS; //16ms per frame

    Uint64 ticks = SDL_GetTicks();
    int actualFrameTime;

    game = new Game();
    game->init("Mafia Revenge", 800, 600, false);

    //game loop
    while (game->running()) {

        Uint64 currentTicks = SDL_GetTicks();
        //essentially the gap when game running on lower than usual FPS
        float deltaTime = (currentTicks - ticks) / 1000.0f;
        ticks = currentTicks;

        game->handleEvents();
        game->update(deltaTime);
        game->render();


        actualFrameTime = SDL_GetTicks() - ticks; //elapsed time in ms it took the curr frame


        //frame limiter
        //keeps the game running at the desired frame rate
        //if actual frame took less time then desired frame, delay the diff
        if (desiredFrameTime > actualFrameTime) {
            SDL_Delay(desiredFrameTime - actualFrameTime);
        }
    }

    delete game;

    return 0;
}
