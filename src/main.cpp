#include <iostream>
#include "Game.hpp"

Game *game = nullptr;

int main() {
    Vector2D a = Vector2D(3, 4);
    Vector2D b = Vector2D(1, 2);
    a=-a;
    std::cout << a.x << "," << a.y << std::endl;
    a=-a;

    a+=b; // a is (4, 6)
    std::cout << a.x << "," << a.y << std::endl;

    Vector2D c = a - b;  // c is (3, 4)
    std::cout << c.x << "," << c.y << std::endl;

    Vector2D d = a * 2;  // d is (8, 12)
    std::cout << d.x << "," << d.y << std::endl;

    Vector2D e = 2 * a;  // e is also (8, 12)
    std::cout << e.x << "," << e.y << std::endl;

    Vector2D f = -a;     // f is (-4, -6)
    std::cout << f.x << "," << f.y << std::endl;

    bool same = (a == b); // false
    std::cout << same << std::endl;

    const int FPS = 60;
    const int desiredFrameTime = 1000 / FPS; //16ms per frame

    Uint64 ticks = SDL_GetTicks();
    int actualFrameTime;

    game = new Game();
    game->init("A1", 800, 600, false);

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
