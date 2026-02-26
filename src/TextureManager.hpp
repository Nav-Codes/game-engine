//
// Created by navjo on 1/14/2026.
//

#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <string>
#include <unordered_map>

#include "SDL3_image/SDL_image.h"

class TextureManager {
private:
    static std::unordered_map<std::string, SDL_Texture*> textures;
public:
    static SDL_Texture* load(const char* path);
    static void draw(SDL_Texture* texture, SDL_FRect src, SDL_FRect dst);
    static void clean();
};

#endif //TEXTUREMANAGER_HPP
