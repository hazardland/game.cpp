#ifndef GAME_RECGANGLE
#define GAME_RECGANGLE

using namespace std;

#include <iostream>

#include <SDL2/SDL_image.h>

#include <game/object.hpp>
#include <game/camera.hpp>

class Rectangle: public Object {
    public:
    SDL_Renderer* renderer;
    SDL_Color color;
    bool visible = true;
    Rectangle(SDL_Renderer* renderer, SDL_Color color={255, 255, 255, 100}) {
        this->renderer = renderer;
        this->color = color;
    }
    void show() {
        visible = true;
    }
    void hide() {
        visible = false;
    }
    void render(Camera* camera) {
        if (visible) {
            //SDL_RenderSetScale(renderer, 5, 5);
            // cout << "Drawing rectangle "<< position.x << " " << position.y << " " << position.w << " " << position.h << "\n";
            // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &position);

            if (color.a<255){
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
                SDL_RenderDrawRect(renderer, &position);
            }


            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            //SDL_RenderSetScale(renderer, 1, 1);
        }
    }
    
};

#endif
