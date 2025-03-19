// File: text.cpp

#include "game/text.h"
#include <iostream>
#include <string>


#include "game/object.h"
#include "game/state.h"
#include "game/camera.h"
#include "game/position.h"

Text::Text(TTF_Font* font, std::string text, int x, int y, SDL_Color color) {
    setPosition(x, y);
    this->color = color;
    this->font = font;
    positionFixed = true;
    // this->renderer = renderer;
}

Text::Text(TTF_Font* font, Position* position, std::string text, SDL_Color color) {
    this->position = position;
    this->color = color;
    this->font = font;
    // this->renderer = renderer;
    positionFixed = false;
}

Text* Text::setText(std::string text) {
    if (this->text==text){
        return this;
    }
    prepared = false;
    this->text = text;
    return this;
}

Text* Text::appendText(std::string text) {
    setText(this->text + text);
    return this;
}

Text* Text::setColor(SDL_Color color) {
    this->color.r = color.r;
    this->color.g = color.g;
    this->color.b = color.b;
    this->color.a = color.a;
    return this;
}

Text* Text::enableCache() {
    cacheEnabled = true;
    return this;
}

void Text::render(State* state, Position* position) {
    if (!visible || text.empty()) {
        return;
    }
    if (!prepared) {
        if (cacheEnabled && cache.contains(text)) {
            //printf("Cache hit %s\n", text);
            texture = cache[text];
            int width;
            int height;
            SDL_QueryTexture(texture, NULL, NULL, &width, &height);
            position->setSize(width, height);
        } else {
            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
            if (surface==NULL) {
                printf("Failed to render text: %s", SDL_GetError());
            }
            position->setSize(surface->w, surface->h);
            if (texture!=nullptr) {
                if (!cacheEnabled) {
                    SDL_DestroyTexture(texture);
                }
            }
            texture = SDL_CreateTextureFromSurface(state->renderer, surface);
            if (cacheEnabled) {
                cache[text] = texture;
            }
            SDL_FreeSurface(surface);
        }
        prepared = true;
    }
    // printf("rendering tex");
    if (positionFixed) {
        SDL_RenderCopyF(state->renderer, texture, NULL, position->getPosition());
    } else {
        SDL_RenderCopyF(state->renderer, texture, NULL, state->camera->translate(position->getPosition()));
    }
}

void Text::render(State* state) {
    render(state, position);
}

Text* Text::setPositionFixed(bool value) {
    positionFixed = value;
    return this;
}

void Text::toggleVisible() {
    visible = !visible;
}


Text::~Text() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    // for (auto &item : cache) {
    //     SDL_DestroyTexture(item.second);
    // }
}
