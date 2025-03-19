#include "game/mouse.h"

void Mouse::reset() {
    rightClick = leftClick =  false;
}

bool Mouse::inside(SDL_FRect* position) {
    if (x>=position->x &&
        y>=position->y &&
        x<=position->x+position->w && 
        y<=position->y+position->h) {
            return true;
        } 
    return false;
}