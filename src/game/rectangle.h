#ifndef GAME_RECGANGLE_H
#define GAME_RECGANGLE_H

using namespace std;
#include <iostream>

#include <SDL2/SDL_image.h>

#include <game/object.h>

class Rectangle: public Object {
    SDL_Renderer* renderer;
    SDL_Color borderColor;
    public:
    bool visible = false;
    Rectangle(SDL_Renderer* renderer, SDL_Color borderColor={255, 255, 255, 255}) {
        this->borderColor = borderColor;
        this->renderer = renderer;
    }
    void render() {
        if (visible) {
            SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
            // cout << "Drawing rectangle "<< position.x << " " << position.y << " " << position.w << " " << position.h << "\n";
            // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &position);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        }
    }
    
};

#endif
