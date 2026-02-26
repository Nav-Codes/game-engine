//
// Created by navjo on 1/14/2026.
//

#include "TextureManager.hpp"
#include "Game.hpp"

#include <iostream>

//doesnt create any extra mem, just grabs ref of global obj that's in main.cpp
extern Game* game;

std::unordered_map<std::string, SDL_Texture*> TextureManager::textures;

//load texture with a path
SDL_Texture *TextureManager::load(const char *path) {

    //caching: storing result of some work so u dont have to repeat work next time
    //check if texture already exists in map

    auto it = textures.find(path);
    if (it != textures.end()) {
        return it->second;
    }

    //surface represents an image in ram (cpu mem)
    //do we want to do anything with image before we store it in gpu?
    SDL_Surface* tempSurface = IMG_Load(path);
    if (!tempSurface) {
        std::cout << "Failed to load image: " << path << std::endl;
    }

    //texture represents image in vram (gpu mem)
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, tempSurface);

    //cleanup surface
    SDL_DestroySurface(tempSurface);

    //check if texture creation was successful
    if (!texture) {
        std::cout << "Failed to create texture: " << path << std::endl;
        return nullptr;
    }

    //store new tex in cache
    textures[path] = texture;

    return texture;
}

void TextureManager::draw(SDL_Texture *texture, SDL_FRect src, SDL_FRect dst) {
    //draw texture to screen from vram
    SDL_RenderTexture(game->renderer, texture, &src, &dst);
}

void TextureManager::clean() {
    for (auto& tex : textures) {
        SDL_DestroyTexture(tex.second);
        tex.second = nullptr;
    }

    textures.clear();
}

