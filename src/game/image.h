#ifndef GAME_IMAGE_H
#define GAME_IMAGE_H

#include <stdio.h>
#include <SDL2/SDL_image.h>

#include <game/util.h>

/**
 * @brief Image class is a base class for preparing textures into GPU RAM
 * It will render region of image at provided position
 * Additionally it can replace colors in source image before transfering to GPU RAM
 */
class Image {
    public:
        SDL_Texture* texture;
        int width;
        int height;

        Image (SDL_Renderer* renderer,
                const char* imagePath,
                const int fromColors[][3] = NULL,
                const int toColors[][3] = NULL
                ) {

            SDL_Surface* surface = IMG_Load(imagePath);
            if (!surface) {
                // @todo: raise error
                printf("Failed to create surface: %s", SDL_GetError());
            }

            // Works onlo on 8 bit colors
            if (fromColors!=NULL && toColors!=NULL) {
                replaceColors(surface, fromColors, toColors);
            }

            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        }

        int getWidth() {
            return width;
        }
        int getHeight() {
            return height;
        }
        void render(SDL_Renderer* renderer, SDL_Rect* frame, SDL_Rect* position, SDL_RendererFlip flip = SDL_FLIP_NONE) {
            // cout << this << " Rendering x:" << _frame->x << " y:" << _frame->y << " w:" << _frame->w << " h:" << _frame->h << " ";
            // cout << "At x:" << position->x << " y:" << position->y << " w:" << position->w << " h:" << position->h << "\n";
            SDL_RenderCopyEx(renderer, texture, frame, position, 0, NULL, flip);
        }
        ~Image() {
            SDL_DestroyTexture(texture);
        }
};


#endif
