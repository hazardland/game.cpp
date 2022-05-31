#ifndef GAME_FRAME_H
#define GAME_FRAME_H

#include <SDL2/SDL_image.h>

/**
 * @brief Defines rectangle of original texture to be used as frame
 * 
 */
class Frame {
    public:
        SDL_Rect rect;
        // int pause;
        bool flipX = false;
        bool flipY = false;
        Frame() {
            rect.x = 0;
            rect.y = 0;
            rect.w = 0;
            rect.h = 0;
        }
        SDL_RendererFlip getFlip(){
            if (flipX && flipY) {
                return (SDL_RendererFlip) (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
            } else if (flipX) {
                return SDL_FLIP_HORIZONTAL;
            } else if (flipY) {
                return SDL_FLIP_VERTICAL;
            }
            return SDL_FLIP_NONE;
        }

        SDL_Rect* getRect(){
            return &rect;
        }
};

#endif
