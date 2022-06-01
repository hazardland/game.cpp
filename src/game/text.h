#ifndef GAME_FONT_H
#define GAME_FONT_H

using namespace std;
#include <string>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Text {
    SDL_Texture* texture = NULL;
    SDL_Rect position;
    TTF_Font* font;
    SDL_Renderer* renderer;
    SDL_Color color;
    string text;
    public: 
    Text(SDL_Renderer* renderer, TTF_Font* font, string text, int x=0, int y=0, SDL_Color color={255, 255, 255}) {
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
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color); 
        position.w = surface->w*100;
        position.h = surface->h*100;
        cout << position.w << "x" << position.h << " ";
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    
    void render() {
                //printf("%s", text.c_str());
        SDL_RenderCopy(renderer, texture, NULL, &position);
    }  

    ~Text() {
        SDL_DestroyTexture(texture);
    }
    
};

#endif
