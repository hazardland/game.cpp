#include "game/frame.h"

Frame::Frame() {
    rect.x = 0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
}

SDL_RendererFlip Frame::getFlip(){
    return flip;
}

void Frame::cacheFlip(){
    if (flipX && flipY) {
        flip = static_cast<SDL_RendererFlip> (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    } else if (flipX) {
        flip = SDL_FLIP_HORIZONTAL;
    } else if (flipY) {
        flip =  SDL_FLIP_VERTICAL;
    } else {
        flip = SDL_FLIP_NONE;
    }
}

SDL_Rect* Frame::getRect(){
    return &rect;
}
