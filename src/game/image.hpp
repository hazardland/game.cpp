#ifndef GAME_IMAGE
#define GAME_IMAGE

#include <stdio.h>
#include <SDL2/SDL_image.h>

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
        SDL_Renderer* renderer;

        Image (SDL_Renderer* renderer, const char* path) {

            this->renderer = renderer;

            // SDL_Surface* surface = IMG_Load(path);
            // if (!surface) {
            //     // @todo: raise error
            //     printf("Failed to load image %s: %s", path, SDL_GetError());
            // }

            // // Works onlo on 8 bit colors
            // if (fromColors!=NULL && toColors!=NULL) {
            //     replaceColors(surface, fromColors, toColors);
            // }

            // texture = SDL_CreateTextureFromSurface(renderer, surface);
            // SDL_FreeSurface(surface);

            texture = IMG_LoadTexture (this->renderer, path);
            
            SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        }

        int getWidth() {
            return width;
        }
        int getHeight() {
            return height;
        }
        void render(SDL_Rect* frame, SDL_Rect* position, SDL_RendererFlip flip = SDL_FLIP_NONE) {
            SDL_RenderCopyEx(renderer, texture, frame, position, 0, NULL, flip);
        }
        ~Image() {
            SDL_DestroyTexture(texture);
        }
};


#endif
