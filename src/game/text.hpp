#ifndef GAME_TEXT
#define GAME_TEXT

using namespace std;
#include <string>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <game/object.hpp>

class Text: public Object {
    SDL_Texture* texture = NULL;
    // SDL_Rect position;
    TTF_Font* font;
    SDL_Renderer* renderer;
    SDL_Color color;
    string text;
    public: 
    Text(SDL_Renderer* renderer, TTF_Font* font, string text="", int x=0, int y=0, SDL_Color color={255, 255, 255}) {
        position.x = x;
        position.y = y;
        this->color = color;
        this->font = font;
        this->renderer = renderer;
    }

    void setText(string text) {
        if (this->text==text){
            return;
        }
        this->text = text;
        SDL_DestroyTexture(texture);
        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
        if (surface==NULL) {
            printf("Failed to render text: %s", SDL_GetError());            
        }
        position.w = surface->w;
        position.h = surface->h;
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    
    void render(State* state) {
        SDL_RenderCopy(renderer, texture, NULL, &position);
    }  

    ~Text() {
        SDL_DestroyTexture(texture);
    }
    
};

#endif
