// File: rectangle.cpp

#include "game/rectangle.h"

#include "game/state.h"

Rectangle::Rectangle(SDL_Color border, SDL_Color background)
: border(border), background(background), visible(true) {}

void Rectangle::show() {
    visible = true;
}

void Rectangle::hide() {
    visible = false;
}

void Rectangle::render(State* state) {
    if (visible) {
        if (background.a>0) {
            SDL_SetRenderDrawColor(state->renderer, background.r, background.g, background.b, background.a);
            SDL_RenderFillRectF(state->renderer, getPosition());
        }

        if (border.a>0){
            SDL_SetRenderDrawColor(state->renderer, border.r, border.g, border.b, border.a);
            SDL_RenderDrawRectF(state->renderer, getPosition());
        }

        if (background.a>0 || border.a>0){
            SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 0);
        }
    }
}
