//
// Created by navjo on 1/14/2026.
//
#include "GameObject.hpp"

#include "TextureManager.hpp"

GameObject::GameObject(const char* path, int x, int y) :
xPos(x), yPos(y), texture(TextureManager::load(path))
{}

GameObject::~GameObject() {
    if (texture) SDL_DestroyTexture(texture);
}

void GameObject::update(float deltaTime) {
    //move game object one pixel on the x and y each frame
    xPos += 60.0f * deltaTime;
    yPos += 60.0f * deltaTime;

    //where do we want to crop the texture from?
    srcRect.x = srcRect.y = 0;

    //how much to crop?
    srcRect.w = srcRect.h = 128;

    //where to draw it?
    dstRect.x = static_cast<float>(xPos);
    dstRect.y = static_cast<float>(yPos);

    // what sizes to render?
    dstRect.h = srcRect.h;
    dstRect.w = srcRect.w;
}

void GameObject::draw() {
    TextureManager::draw(texture, srcRect, dstRect);
}


