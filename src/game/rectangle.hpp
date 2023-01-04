#ifndef GAME_RECGANGLE
#define GAME_RECGANGLE

using namespace std;

#include <iostream>

#include <SDL2/SDL_image.h>

#include <game/object.hpp>
#include <game/state.hpp>

class Rectangle: public Object {

    public:

    SDL_Renderer* renderer;
    SDL_Color background;
    SDL_Color border;
    int alpha;
    bool visible = true;

    Rectangle(SDL_Renderer* renderer, SDL_Color border={}, SDL_Color background={}) {
        this->renderer = renderer;
        //this->color = SDL_Color(red, blue, green, alpha);
        this->background = background;
        this->border = border;

    }
    // Rectangle(SDL_Renderer* renderer, SDL_Color* color={255, 255, 255, 100}) {
    //     this->renderer = renderer;
    //     this->color = color;
    // }
    void show() {
        visible = true;
    }
    void hide() {
        visible = false;
    }
    void render(State* state) {
        if (visible) {
            //SDL_RenderSetScale(renderer, 5, 5);
            // cout << "Drawing rectangle "<< position.x << " " << position.y << " " << position.w << " " << position.h << "\n";
            // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

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
            //SDL_RenderSetScale(renderer, 1, 1);
        }
    }
    
};

#endif
