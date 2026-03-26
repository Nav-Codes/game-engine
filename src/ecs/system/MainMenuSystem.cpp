//
// Created by navjo on 3/25/2026.
//
#include "MainMenuSystem.hpp"

#include "Game.hpp"

void MainMenuSystem::update(const SDL_Event &event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch(event.key.key) {
            case SDLK_SPACE :
                Game::onSceneChangeRequest("level1");
                break;
            default : break;
        }
    }
}
