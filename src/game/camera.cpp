#include "game/camera.h"

void Camera::setSize(int width, int height) {
    this->width = width;
    this->height = height;
}

bool Camera::isVisible(SDL_FRect* position) {
    if (position->x + position->w  > x && 
        position->y + position->h > y &&
        position->x < x + width && 
        position->y < y + height
    ) {
        return true;
    }
    return false;
}

SDL_FRect* Camera::translate(SDL_FRect* position) {
    result.x = position->x - x; 
    result.y = position->y - y;
    result.w = position->w;
    result.h = position->h;
    return &result; 
}
