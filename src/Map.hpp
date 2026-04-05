//
// Created by navjo on 1/14/2026.
//

#ifndef MAP_HPP
#define MAP_HPP

// #include "Game.hpp"

#include <vector>
#include <SDL3/SDL.H>
#include <Component.hpp>

#include "tinyxml2.h"
#include "Collision.hpp"

class Map {
public:
    Map() = default;
    ~Map() = default;

    void load(const char *path, SDL_Texture *ts);
    void draw(const Camera& cam);

    SDL_Texture *tileset = nullptr;
    int width{}, height{};
    std::vector<std::vector<int>> tileData;
    std::vector<Collider> regularColliders;
    std::vector<Collider> carColliders;
    std::vector<Collider> enemySpawnPoints;
    std::vector<Collider> carSpawnPoint;
    std::vector<Collider> playerSpawnPoint;

private:
    void addToList(vector<Collider>& collisionList, tinyxml2::XMLElement* elem);
};

#endif //MAP_HPP
