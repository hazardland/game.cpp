// File: rectangle.cpp

#include "game/rectangle.h"

#include "game/state.h"

Rectangle::Rectangle(SDL_Renderer* renderer, SDL_Color border, SDL_Color background)
: renderer(renderer), border(border), background(background), visible(true) {}

void Rectangle::show() {
    visible = true;
}

void Rectangle::hide() {
    visible = false;
}

void Rectangle::render(State* state) {
    if (visible) {
        if (background.a>0) {
            SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
            SDL_RenderFillRectF(renderer, getPosition());
        }

        if (border.a>0){
            SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
            SDL_RenderDrawRectF(renderer, getPosition());
        }

        if (background.a>0 || border.a>0){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }
    }
}
