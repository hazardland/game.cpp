#ifndef GAME_TEXT
#define GAME_TEXT

using namespace std;
#include <string>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <game/object.hpp>

class Text: public Object {
    public: 
    SDL_Texture* texture = NULL;
    // SDL_Rect position;
    TTF_Font* font;
    SDL_Renderer* renderer;
    SDL_Color color;
    string text;
    map<string, SDL_Texture*> cache;
    Text(SDL_Renderer* renderer, TTF_Font* font, string text="", int x=0, int y=0, SDL_Color color={255, 255, 255}) {
        setPosition(x, y);
        this->color = color;
        this->font = font;
        this->renderer = renderer;
    }

    void setText(string text) {
        if (this->text==text){
            return;
        }
            if (cache.contains(text)) {
                //printf("Cache hit %s\n", text);
                texture = cache[text];
                this->text = text;
                int width;
                int height;
                SDL_QueryTexture(texture, NULL, NULL, &width, &height);
                setSize(width, height);
                return;
            }
        this->text = text;
        // SDL_DestroyTexture(texture);
        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
        if (surface==NULL) {
            printf("Failed to render text: %s", SDL_GetError());            
        }
        setSize(surface->w, surface->h);
        texture = NULL;
        cache[text] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        texture = cache[text];
    }

    void setColor(int red, int green, int blue) {
        this->color.r = red;
        this->color.g = green;
        this->color.b = blue;
    }
    
    void render(State* state) {
        SDL_RenderCopyF(renderer, texture, NULL, getPosition());
    }  

    ~Text() {
        SDL_DestroyTexture(texture);
    }
    
};

#endif
