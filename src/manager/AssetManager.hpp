//
// Created by navjo on 2/18/2026.
//

#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP
#include <string>
#include <unordered_map>

#include "Component.hpp"

using namespace std;

class AssetManager {
    static unordered_map<string, Animation> animations;
    static Animation loadAnimaitonFromXML(const char* path);
public:
    static void loadAnimation(const string& clipName, const char* path);
    static const Animation& getAnimation(const string& clipName);
};

#endif //ASSETMANAGER_HPP
