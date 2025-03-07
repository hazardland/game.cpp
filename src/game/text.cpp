// File: text.cpp

#include "game/text.h"
#include <iostream>
#include <string>


#include "game/object.h"
#include "game/state.h"

Text::Text(SDL_Renderer* renderer, TTF_Font* font, std::string text, int x, int y, SDL_Color color) {
    setPosition(x, y);
    this->color = color;
    this->font = font;
    this->renderer = renderer;
}

void Text::setText(std::string text) {
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

void Text::setColor(int red, int green, int blue) {
    color.r = red;
    color.g = green;
    color.b = blue;
}

void Text::render(State* state) {
    SDL_RenderCopyF(renderer, texture, NULL, getPosition());
}

Text::~Text() {
    SDL_DestroyTexture(texture);
    for (auto &item : cache) {
        SDL_DestroyTexture(item.second);
    }
}
