#ifndef GAME_FRAME
#define GAME_FRAME

#include <SDL2/SDL_image.h>

/**
 * @brief Defines rectangle of original texture to be used as frame
 * 
 */
class Frame {
    public:
        SDL_Rect rect;
        bool flipX = false;
        bool flipY = false;
        SDL_RendererFlip flip;
        Frame() {
            rect.x = 0;
            rect.y = 0;
            rect.w = 0;
            rect.h = 0;
        }
        SDL_RendererFlip getFlip(){
            return flip;
        }

        void cacheFlip(){
            if (flipX && flipY) {
                flip = (SDL_RendererFlip) (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
            } else if (flipX) {
                flip = SDL_FLIP_HORIZONTAL;
            } else if (flipY) {
                flip =  SDL_FLIP_VERTICAL;
            } else {
                flip = SDL_FLIP_NONE;
            }
        }

        SDL_Rect* getRect(){
            return &rect;
        }
};

#endif
