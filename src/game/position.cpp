// File: position.cpp

#include "game/position.h"

#include "game/camera.h"


Position::Position(float x, float y, float width, float height,
                   float* parentX, float* parentY,
                   float* parentWidth, float* parentHeight) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->parentX = parentX;
    this->parentY = parentY;
    this->parentWidth = parentWidth;
    this->parentHeight = parentHeight;
    this->parentWidthRatio = *parentWidth / width;
    this->parentHeightRatio = *parentHeight / height;
}

float Position::getX() {
    return *parentX + (x * ((*parentWidth / width) / parentWidthRatio));
}

float Position::getY() {
    return *parentY + (y * ((*parentHeight / height) / parentHeightRatio));
}

float Position::getWidth() {
    return width * ((*parentWidth / width) / parentWidthRatio);
}

float Position::getHeight() {
    return height * ((*parentHeight / height) / parentHeightRatio);
}

SDL_FRect* Position::getSDL_FRect() {
    rect.x = getX();
    rect.y = getY();
    rect.w = getWidth();
    rect.h = getHeight();
    return &rect;
}

void Position::draw(State* state) {
    if (color.a > 0){
        SDL_SetRenderDrawColor(state->renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRectF(state->renderer, state->camera->translate(getSDL_FRect()));
        SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 0);
    }
}
