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
    recalculateIfNeeded();
    return rect.x;
}

float Position::getY() {
    recalculateIfNeeded();
    return rect.y;
}

float Position::getWidth() {
    recalculateIfNeeded();
    return rect.w;
}

float Position::getHeight() {
    recalculateIfNeeded();
    return rect.h;
}

SDL_FRect* Position::getSDL_FRect() {
    recalculateIfNeeded();
    return &rect;
}

void Position::recalculateIfNeeded() {
    if (needsUpdate){
        rect.x = *parentX + (x * ((*parentWidth / width) / parentWidthRatio));
        rect.y = *parentY + (y * ((*parentHeight / height) / parentHeightRatio));
        rect.w = width * ((*parentWidth / width) / parentWidthRatio);
        rect.h = height * ((*parentHeight / height) / parentHeightRatio);
        needsUpdate = false;
    }
}

void Position::setRequiresUpdate() {
    needsUpdate = true;
}

void Position::draw(State* state) {
    if (color.a > 0){
        SDL_SetRenderDrawColor(state->renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawRectF(state->renderer, state->camera->translate(getSDL_FRect()));
        SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 0);
    }
}
