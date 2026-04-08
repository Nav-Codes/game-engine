//
// Created by navjo on 1/14/2026.
//

#ifndef MAP_HPP
#define MAP_HPP


#include <vector>
#include <Component.hpp>

#include "tinyxml2.h"

struct Tileset {
    std::string source; //tileset.tsx
    int firstGid = 1;

    int tileWidth = 16;
    int tileHeight = 16;
    int columns = 1;
    int tileCount = 0;

    SDL_Texture* texture = nullptr;
};

struct TileLayer {
    std::string name;
    int width = 0;
    int height = 0;
    std::vector<unsigned> data; // width * height gids
};

struct RectObject {
    std::string name;
    float x = 0.f;
    float y = 0.f;
    float w = 0.f;
    float h = 0.f;
};

struct PointObject {
    std::string name;
    float x = 0.f;
    float y = 0.f;
};

class Map {
public:
    bool loadFromTMX(const std::string& path);
    void draw(const Camera& cam) const;

    std::vector<Collider> playerColliders;
    std::vector<Collider> carColliders;
    std::vector<Collider> enemySpawnPoints;
    std::vector<Collider> playerSpawnPoint;
    std::vector<Collider> carSpawnPoint;

    void drawLayer(const TileLayer& layer, const Camera& cam) const;
    bool loadTSX(const std::string& tsxPath, int firstGid);
    void addToList(vector<Collider>& collisionList, tinyxml2::XMLElement* elem);

    int width = 0;
    int height = 0;
    int tileWidth = 16;
    int tileHeight = 16;

    Tileset tileset;
    std::vector<TileLayer> layers;

};

#endif //MAP_HPP
