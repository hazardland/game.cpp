#ifndef GAME_IMAGE
#define GAME_IMAGE

#include <stdio.h>
#include <SDL2/SDL_image.h>

#include <game/util.hpp>

/**
 * @brief Image class loads and stores texture into memory for further use
 * It can later render region of image to a specific position into scene
 */
class Image {
    private:
        int width;
        int height;

    public:
        SDL_Texture* texture;
        SDL_Renderer* renderer;

        Image (SDL_Renderer* renderer, const char* imagePath) {
            this->renderer = renderer;
            texture = IMG_LoadTexture (this->renderer, imagePath);
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
