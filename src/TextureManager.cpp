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

void TextureManager::loadLabel(Label &label) {
    auto it = textures.find(label.textureCacheKey);
    if (it != textures.end()) {
        label.texture = it->second;
        return;
    }

    updateLabel(label);
}

void TextureManager::updateLabel(Label &label) {
    if (!label.dirty) return;

    if (label.texture) {
        SDL_DestroyTexture(label.texture);
        label.texture = nullptr;
    }

    SDL_Surface* tempSurface = TTF_RenderText_Blended (
        label.font,
        label.text.c_str(),
        label.text.size(),
        label.color
    );

    if (!tempSurface) {
        std::cerr << "Failed to load surface: " << label.textureCacheKey << endl;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, tempSurface);
    SDL_DestroySurface(tempSurface);

    if (!texture) {
        std::cerr << "Failed to load surface: " << label.textureCacheKey << endl;
    }

    float w,h;
    SDL_GetTextureSize(texture, &w, &h);
    label.dst.w = w;
    label.dst.h = h;

    //cache
    label.texture = texture;
    textures[label.textureCacheKey] = texture;

    label.dirty = false; //clean

}


void TextureManager::draw(SDL_Texture *texture, const SDL_FRect* src, const SDL_FRect* dst) {
    //draw texture to screen from vram
    SDL_RenderTexture(game->renderer, texture, src, dst);
}

void TextureManager::draw(SDL_Texture *texture, SDL_FRect src, SDL_FRect dst, float rotation, SDL_FPoint center) {
    SDL_RenderTextureRotated(game->renderer, texture, &src, &dst, rotation, &center, SDL_FLIP_NONE);
}

void TextureManager::clean() {
    for (auto& tex : textures) {
        SDL_DestroyTexture(tex.second);
        tex.second = nullptr;
    }

    textures.clear();
}

