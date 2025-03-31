// File: image.cpp

#include "game/image.h"

Image::Image(SDL_Renderer* renderer, const char* path) {
    this->renderer = renderer;
    texture = IMG_LoadTexture(this->renderer, path);
    // SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_GetTextureSize(texture, &width, &height);
}

Image::~Image() {
    SDL_DestroyTexture(texture);
}

int Image::getWidth() {
    return width;
}

int Image::getHeight() {
    return height;
}

void Image::render(SDL_FRect* frame, SDL_FRect* position, SDL_FlipMode flip) {
    SDL_RenderTextureRotated(renderer, texture, frame, position, 0, NULL, flip);
}
