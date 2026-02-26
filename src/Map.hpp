//
// Created by navjo on 1/14/2026.
//

#ifndef MAP_HPP
#define MAP_HPP

// #include "Game.hpp"

#include <vector>
#include <SDL3/SDL.H>
#include <Component.hpp>

class Map {
public:
    Map() = default;
    ~Map() = default;

    void load(const char *path, SDL_Texture *ts);
    void draw(const Camera& cam);

    SDL_Texture *tileset = nullptr;
    int width{}, height{};
    std::vector<std::vector<int>> tileData;
    std::vector<Collider> colliders;
    std::vector<Collider> spawnPoints;
};

#endif //MAP_HPP
