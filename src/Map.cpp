//
// Created by navjo on 1/14/2026.
//

#include "Map.hpp"

#include <cmath>

#include "TextureManager.hpp"
#include <sstream>
#include <tinyxml2.h>

void Map::load(const char *path, SDL_Texture *ts) {
    tileset = ts;
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    //parse width and height
    auto* mapNode = doc.FirstChildElement("map");
    width = mapNode->IntAttribute("width");
    height = mapNode->IntAttribute("height");

    //parse terrain data
    auto* layer = mapNode->FirstChildElement("layer");
    auto* data = layer->FirstChildElement("data");

    std::string csv = data->GetText();
    std::stringstream ss(csv);
    tileData = std::vector(height, std::vector<int>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::string val;
            //read characters from a ss into val until it hits a comma or is at the end of the stream
            if (!std::getline(ss, val, ',')) break;
            tileData[i][j] = std::stoi(val); //
        }
    }

    //parse collider data
    // auto* objectGroup = layer->NextSiblingElement("objectgroup");

    tinyxml2::XMLElement* objectGroup = nullptr;

    for (auto* elem = mapNode->FirstChildElement("objectgroup");
         elem != nullptr;
         elem = elem->NextSiblingElement("objectgroup"))
    {
        const char* name = elem->Attribute("name");

        if (name && std::string(name) == "Collision Layer")
        {
            objectGroup = elem;
            break;
        }
    }

    //create loop w init condition and increment
    for (auto* obj = objectGroup->FirstChildElement("object"); obj != nullptr; obj = obj->NextSiblingElement("object")) {
        Collider c;
        c.rect.x = obj->FloatAttribute("x");
        c.rect.y = obj->FloatAttribute("y");
        c.rect.h = obj->FloatAttribute("height");
        c.rect.w = obj->FloatAttribute("width");
        colliders.push_back(c);
    }

    for (auto* elem = mapNode->FirstChildElement("objectgroup");
             elem != nullptr;
             elem = elem->NextSiblingElement("objectgroup"))
    {
        const char* name = elem->Attribute("name");

        if (name && std::string(name) == "Spawn Layer")
        {
            objectGroup = elem;
            break;
        }
    }

    //parse coin spawn points
    // auto* coinObjectGroup = objectGroup->NextSiblingElement("objectgroup");
    for (auto* obj = objectGroup->FirstChildElement("object"); obj != nullptr; obj = obj->NextSiblingElement("object")) {
        Collider c;
        c.rect.x = obj->FloatAttribute("x");
        c.rect.y = obj->FloatAttribute("y");
        c.rect.h = obj->FloatAttribute("height");
        c.rect.w = obj->FloatAttribute("width");
        spawnPoints.push_back(c);
    }
}


void Map::draw(const Camera& cam) {
    SDL_FRect src{}, dest{};

    dest.w = dest.h = 32;

    for (int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            int type = tileData[row][col];

            float worldX = static_cast<float>(col) * dest.w;
            float worldY = static_cast<float>(row) * dest.h;

            dest.x = std::round(worldX - cam.view.x);
            dest.y = std::round(worldY - cam.view.y);

            switch (type) {
                case 1: //dirt
                    src.x = 0;
                    src.y = 0;
                    src.h = 32;
                    src.w = 32;
                    break;
                case 2: //grass
                    src.x = 32;
                    src.y = 0;
                    src.h = 32;
                    src.w = 32;
                    break;
                case 4: //water
                    src.x = 32;
                    src.y = 32;
                    src.h = 32;
                    src.w = 32;
                    break;
                default: break;
            }

            TextureManager::draw(tileset, src, dest);
        }
    }
}
