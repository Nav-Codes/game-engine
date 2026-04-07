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
    static unordered_map<string, TTF_Font*> fonts;
    static Animation loadAnimaitonFromXML(const char* path);
public:
    static void loadFont(const string& name, const char* path, float fontSize);
    static TTF_Font* getFont(const std::string& name);
    static void loadAnimation(const string& clipName, const char* path);
    static const Animation& getAnimation(const string& clipName);
};

#endif //ASSETMANAGER_HPP
