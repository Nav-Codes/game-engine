//
// Created by navjo on 1/14/2026.
//

#include "Map.hpp"

#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include "tinyxml2.h"

#include "manager/TextureManager.hpp"
#include "Game.hpp"

using namespace tinyxml2;

static std::vector<unsigned> parseCSV(const std::string& csv) {
    std::vector<unsigned> result;
    std::stringstream ss(csv);
    std::string item;

    while (std::getline(ss, item, ',')) {
        // trim whitespace/newlines
        item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
        if (item.empty()) continue;
        result.push_back(static_cast<unsigned>(std::stoul(item)));
    }

    return result;
}

bool Map::loadFromTMX(const std::string& path) {
    XMLDocument doc;
    if (doc.LoadFile(path.c_str()) != XML_SUCCESS) {
        return false;
    }

    XMLElement* mapElem = doc.FirstChildElement("map");
    if (!mapElem) return false;

    width = mapElem->IntAttribute("width");
    height = mapElem->IntAttribute("height");
    tileWidth = mapElem->IntAttribute("tilewidth");
    tileHeight = mapElem->IntAttribute("tileheight");

    // --- tileset ---
    XMLElement* tilesetElem = mapElem->FirstChildElement("tileset");
    if (tilesetElem) {
        int firstGid = tilesetElem->IntAttribute("firstgid");
        const char* source = tilesetElem->Attribute("source");

        if (source) {
            std::filesystem::path tmxPath(path);
            std::filesystem::path tsxPath = tmxPath.parent_path() / source;
            if (!loadTSX(tsxPath.string(), firstGid)) {
                return false;
            }
        }
    }

    // --- tile layers ---
    for (XMLElement* layerElem = mapElem->FirstChildElement("layer");
         layerElem;
         layerElem = layerElem->NextSiblingElement("layer")) {

        TileLayer layer;
        if (const char* name = layerElem->Attribute("name")) {
            layer.name = name;
        }

        layer.width = layerElem->IntAttribute("width");
        layer.height = layerElem->IntAttribute("height");

        XMLElement* dataElem = layerElem->FirstChildElement("data");
        if (!dataElem) continue;

        const char* encoding = dataElem->Attribute("encoding");
        if (!encoding || std::string(encoding) != "csv") {
            continue; // only handling CSV for now
        }

        const char* csvText = dataElem->GetText();
        if (!csvText) continue;

        layer.data = parseCSV(csvText);
        layers.push_back(std::move(layer));
    }

    tinyxml2::XMLElement* objectGroup = nullptr;

    //load wall colliders and spawn points
    for (auto* elem = mapElem->FirstChildElement("objectgroup"); elem != nullptr; elem = elem->NextSiblingElement("objectgroup")) {
        const char* name = elem->Attribute("name");
        string strName = string(name);

        //load car wall colliders
        if (name && std::string(name) == "Car Collision Layer") {
            addToList(carColliders, elem);
        }

        //load player wall colliders
        if (name && std::string(name) == "Player Collision Layer") {
            addToList(playerColliders, elem);
        }

        //load enemy spawn points
        if (name && std::string(name) == "Enemy Spawn Points") {
            addToList(enemySpawnPoints, elem);
        }

        //load car spawn point
        if (name && std::string(name) == "Car Spawn Point") {
            addToList(carSpawnPoint, elem);
        }

        //load player spawn point
        if (name && std::string(name) == "Player Spawn Point") {
            addToList(playerSpawnPoint, elem);
        }
    }

    return true;
}

bool Map::loadTSX(const std::string& tsxPath, int firstGid) {
    XMLDocument doc;
    if (doc.LoadFile(tsxPath.c_str()) != XML_SUCCESS) {
        return false;
    }

    XMLElement* tsElem = doc.FirstChildElement("tileset");
    if (!tsElem) return false;

    tileset.firstGid = firstGid;
    tileset.tileWidth = tsElem->IntAttribute("tilewidth");
    tileset.tileHeight = tsElem->IntAttribute("tileheight");
    tileset.tileCount = tsElem->IntAttribute("tilecount");
    tileset.columns = tsElem->IntAttribute("columns");

    XMLElement* imageElem = tsElem->FirstChildElement("image");
    if (!imageElem) return false;

    const char* imageSource = imageElem->Attribute("source");
    if (!imageSource) return false;

    std::filesystem::path p(tsxPath);
    std::filesystem::path imagePath = p.parent_path() / imageSource;

    tileset.source = imagePath.string();
    tileset.texture = TextureManager::load(tileset.source.c_str());

    return tileset.texture != nullptr;
}

void Map::draw(const Camera& cam) const {
    for (const auto& layer : layers) {
        drawLayer(layer, cam);
    }
}

void Map::drawLayer(const TileLayer& layer, const Camera& cam) const {
    if (!tileset.texture) return;

    int startCol = std::max(0, static_cast<int>(cam.view.x) / tileWidth);
    int endCol = std::min(layer.width, static_cast<int>(cam.view.x + cam.view.w) / tileWidth + 1);

    int startRow = std::max(0, static_cast<int>(cam.view.y) / tileHeight);
    int endRow = std::min(layer.height, static_cast<int>(cam.view.y + cam.view.h) / tileHeight + 1);

    SDL_FRect src{}, dest{};
    src.w = static_cast<float>(tileset.tileWidth);
    src.h = static_cast<float>(tileset.tileHeight);
    dest.w = static_cast<float>(tileWidth);
    dest.h = static_cast<float>(tileHeight);

    for (int row = startRow; row < endRow; ++row) {
        for (int col = startCol; col < endCol; ++col) {
            unsigned gid = layer.data[row * layer.width + col];
            if (gid == 0) continue; // empty tile

            unsigned localId = gid - tileset.firstGid;

            src.x = static_cast<float>((localId % tileset.columns) * tileset.tileWidth);
            src.y = static_cast<float>((localId / tileset.columns) * tileset.tileHeight);

            dest.x = std::round(col * tileWidth - cam.view.x);
            dest.y = std::round(row * tileHeight - cam.view.y);

            // SDL_RenderTexture(Game::renderer, tileset.texture, &src, &dest);
            TextureManager::draw(tileset.texture, &src, &dest);
        }
    }
}

// void Map::load(const char *path, SDL_Texture *ts) {
//     tileset = ts;
//     tinyxml2::XMLDocument doc;
//     doc.LoadFile(path);
//
//     //parse width and height
//     auto* mapNode = doc.FirstChildElement("map");
//     width = mapNode->IntAttribute("width");
//     height = mapNode->IntAttribute("height");
//
//     //parse terrain data
//     auto* layer = mapNode->FirstChildElement("layer");
//     auto* data = layer->FirstChildElement("data");
//
//     std::string csv = data->GetText();
//     std::stringstream ss(csv);
//     tileData = std::vector(height, std::vector<int>(width));
//     for (int i = 0; i < height; i++) {
//         for (int j = 0; j < width; j++) {
//             std::string val;
//             //read characters from a ss into val until it hits a comma or is at the end of the stream
//             if (!std::getline(ss, val, ',')) break;
//             tileData[i][j] = std::stoi(val); //
//         }
//     }
//
//     //parse collider data
//     // auto* objectGroup = layer->NextSiblingElement("objectgroup");
//
//     tinyxml2::XMLElement* objectGroup = nullptr;
//
//     //load wall colliders and spawn points
//     for (auto* elem = mapNode->FirstChildElement("objectgroup"); elem != nullptr; elem = elem->NextSiblingElement("objectgroup")) {
//         const char* name = elem->Attribute("name");
//         string strName = string(name);
//
//         //load car wall colliders
//         if (name && std::string(name) == "Car Collision Layer") {
//             addToList(carColliders, elem);
//         }
//
//         //load player wall colliders
//         if (name && std::string(name) == "Player Collision Layer") {
//             addToList(playerColliders, elem);
//         }
//
//         //load enemy spawn points
//         if (name && std::string(name) == "Enemy Spawn Points") {
//             addToList(enemySpawnPoints, elem);
//         }
//
//         //load car spawn point
//         if (name && std::string(name) == "Car Spawn Point") {
//             addToList(carSpawnPoint, elem);
//         }
//
//         //load player spawn point
//         if (name && std::string(name) == "Player Spawn Point") {
//             addToList(playerSpawnPoint, elem);
//         }
//     }
// }
//
// void Map::draw(const Camera& cam) {
//     SDL_FRect src{}, dest{};
//     dest.w = static_cast<float>(tileset->w);
//     dest.h = static_cast<float>(tileset->h);
//
//     for (int row = 0; row < height; ++row) {
//         for (int col = 0; col < width; ++col) {
//             int gid = tileData[row][col];
//
//             if (gid == 0) {
//                 continue; // empty tile in TMX
//             }
//
//             int localId = gid - tileset->refcount;
//
//             src.w = static_cast<float>(tileset.tileWidth);
//             src.h = static_cast<float>(tileset.tileHeight);
//             src.x = static_cast<float>((localId % tileset.columns) * tileset.tileWidth);
//             src.y = static_cast<float>((localId / tileset.columns) * tileset.tileHeight);
//
//             float worldX = static_cast<float>(col * tileset.tileWidth);
//             float worldY = static_cast<float>(row * tileset.tileHeight);
//
//             dest.x = std::round(worldX - cam.view.x);
//             dest.y = std::round(worldY - cam.view.y);
//
//             SDL_RenderTextureF(Game::renderer, tileset.texture, &src, &dest);
//         }
//     }
// }

void Map::addToList(vector<Collider>& collisionList, tinyxml2::XMLElement* elem) {
    for (auto* obj = elem->FirstChildElement("object"); obj != nullptr; obj = obj->NextSiblingElement("object")) {
        Collider c;
        c.rect.x = obj->FloatAttribute("x");
        c.rect.y = obj->FloatAttribute("y");
        c.rect.h = obj->FloatAttribute("height");
        c.rect.w = obj->FloatAttribute("width");
        collisionList.push_back(c);
    }
}