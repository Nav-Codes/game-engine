//
// Created by navjo on 1/14/2026.
//

#include "Map.hpp"

#include <cmath>
#include <iostream>
#include <memory>

#include "TextureManager.hpp"
#include <sstream>



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

    //car walls
    for (auto* elem = mapNode->FirstChildElement("objectgroup"); elem != nullptr; elem = elem->NextSiblingElement("objectgroup")) {
        const char* name = elem->Attribute("name");
        string strName = string(name);

        if (name && std::string(name) == "Car Collision Layer") {
            addToList(carColliders, elem);
        }

        if (name && std::string(name) == "Player Collision Layer") {
            addToList(regularColliders, elem);
        }

        if (name && std::string(name) == "Enemy Spawn Points") {
            addToList(enemySpawnPoints, elem);
        }

        if (name && std::string(name) == "Car Spawn Point") {
            addToList(carSpawnPoint, elem);
        }

        if (name && std::string(name) == "Player Spawn Point") {
            addToList(playerSpawnPoint, elem);
        }
    }
}


void Map::draw(const Camera& cam) {
    SDL_FRect src{}, dest{};

    dest.w = dest.h = 16;

    for (int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            int type = tileData[row][col];

            float worldX = static_cast<float>(col) * dest.w;
            float worldY = static_cast<float>(row) * dest.h;

            dest.x = std::round(worldX - cam.view.x);
            dest.y = std::round(worldY - cam.view.y);

            switch (type) {
                case 2436:
                    src.x = 48;
                    src.y = 608;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2582:
                    src.x = 336;
                    src.y = 640;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 3270:
                    src.x = 80;
                    src.y = 816;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2708:
                    src.x = 304;
                    src.y = 672;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 712:
                    src.x = 112;
                    src.y = 176;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 713:
                    src.x = 128;
                    src.y = 176;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 714:
                    src.x = 144;
                    src.y = 176;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 776:
                    src.x = 112;
                    src.y = 192;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 777:
                    src.x = 128;
                    src.y = 192;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 778:
                    src.x = 144;
                    src.y = 192;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 840:
                    src.x = 112;
                    src.y = 208;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 841:
                    src.x = 128;
                    src.y = 208;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 842:
                    src.x = 144;
                    src.y = 208;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1102:
                    src.x = 208;
                    src.y = 272;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1103:
                    src.x = 224;
                    src.y = 272;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1166:
                    src.x = 208;
                    src.y = 288;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1167:
                    src.x = 224;
                    src.y = 288;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1230:
                    src.x = 208;
                    src.y = 304;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1231:
                    src.x = 224;
                    src.y = 304;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2073:
                    src.x = 384;
                    src.y = 512;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2137:
                    src.x = 384;
                    src.y = 528;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2075:
                    src.x = 384;
                    src.y = 512;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2139:
                    src.x = 384;
                    src.y = 528;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 3074:
                    src.x = 16;
                    src.y = 768;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 3078:
                    src.x = 80;
                    src.y = 768;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 3082:
                    src.x = 144;
                    src.y = 768;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 3202:
                    src.x = 16;
                    src.y = 864;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 3586:
                    src.x = 16;
                    src.y = 896;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 3588:
                    src.x = 48;
                    src.y = 896;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 3594:
                    src.x = 144;
                    src.y = 896;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 3210:
                    src.x = 144;
                    src.y = 800;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 3268:
                    src.x = 48;
                    src.y = 816;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 723:
                    src.x = 288;
                    src.y = 176;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 724:
                    src.x = 304;
                    src.y = 176;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 725:
                    src.x = 320;
                    src.y = 176;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 787:
                    src.x = 288;
                    src.y = 192;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 788:
                    src.x = 304;
                    src.y = 192;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 789:
                    src.x = 320;
                    src.y = 192;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 851:
                    src.x = 288;
                    src.y = 208;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 852:
                    src.x = 304;
                    src.y = 208;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 853:
                    src.x = 320;
                    src.y = 208;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1096:
                    src.x = 112;
                    src.y = 272;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1097:
                    src.x = 128;
                    src.y = 272;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1098:
                    src.x = 144;
                    src.y = 272;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1160:
                    src.x = 112;
                    src.y = 288;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1161:
                    src.x = 128;
                    src.y = 288;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1162:
                    src.x = 144;
                    src.y = 288;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1224:
                    src.x = 112;
                    src.y = 304;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1225:
                    src.x = 128;
                    src.y = 304;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1226:
                    src.x = 144;
                    src.y = 304;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2690:
                    src.x = 16;
                    src.y = 672;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2450:
                    src.x = 272;
                    src.y = 608;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 670:
                    src.x = 464;
                    src.y = 160;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2438:
                    src.x = 80;
                    src.y = 608;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2566:
                    src.x = 80;
                    src.y = 640;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2694:
                    src.x = 80;
                    src.y = 672;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 706:
                    src.x = 16;
                    src.y = 176;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 707:
                    src.x = 32;
                    src.y = 176;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 708:
                    src.x = 48;
                    src.y = 176;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 709:
                    src.x = 64;
                    src.y = 176;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 770:
                    src.x = 16;
                    src.y = 192;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 771:
                    src.x = 32;
                    src.y = 192;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 772:
                    src.x = 48;
                    src.y = 192;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 773:
                    src.x = 64;
                    src.y = 192;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 834:
                    src.x = 16;
                    src.y = 208;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 835:
                    src.x = 32;
                    src.y = 208;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 836:
                    src.x = 48;
                    src.y = 208;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 837:
                    src.x = 64;
                    src.y = 208;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1090:
                    src.x = 16;
                    src.y = 272;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1091:
                    src.x = 32;
                    src.y = 272;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1092:
                    src.x = 48;
                    src.y = 272;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1093:
                    src.x = 64;
                    src.y = 272;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1154:
                    src.x = 16;
                    src.y = 288;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1155:
                    src.x = 32;
                    src.y = 288;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1156:
                    src.x = 48;
                    src.y = 288;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1157:
                    src.x = 64;
                    src.y = 288;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1218:
                    src.x = 16;
                    src.y = 304;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1219:
                    src.x = 32;
                    src.y = 304;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1220:
                    src.x = 48;
                    src.y = 304;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 1221:
                    src.x = 64;
                    src.y = 304;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2562:
                    src.x = 16;
                    src.y = 640;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 141:
                    src.x = 192;
                    src.y = 32;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 142:
                    src.x = 208;
                    src.y = 32;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 143:
                    src.x = 224;
                    src.y = 32;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 205:
                    src.x = 192;
                    src.y = 48;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 206:
                    src.x = 208;
                    src.y = 48;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 207:
                    src.x = 224;
                    src.y = 48;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 269:
                    src.x = 192;
                    src.y = 64;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 270:
                    src.x = 208;
                    src.y = 64;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 271:
                    src.x = 224;
                    src.y = 64;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 333:
                    src.x = 192;
                    src.y = 80;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 334:
                    src.x = 208;
                    src.y = 80;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 335:
                    src.x = 224;
                    src.y = 80;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 397:
                    src.x = 192;
                    src.y = 96;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 398:
                    src.x = 208;
                    src.y = 96;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 399:
                    src.x = 224;
                    src.y = 96;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 461:
                    src.x = 192;
                    src.y = 112;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 462:
                    src.x = 208;
                    src.y = 112;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 463:
                    src.x = 224;
                    src.y = 112;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 2448:
                    src.x = 240;
                    src.y = 608;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 544:
                    src.x = 496;
                    src.y = 128;
                    src.h = 16;
                    src.w = 16;
                    break;
                case 542:
                    src.x = 464;
                    src.y = 128;
                    src.h = 16;
                    src.w = 16;
                    break;
                default: break;
            }

            TextureManager::draw(tileset, src, dest);
        }
    }
}

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