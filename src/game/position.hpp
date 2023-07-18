#ifndef GAME_POSITION
#define GAME_POSITION

using namespace std;
#include <iostream>

#include <SDL2/SDL_image.h>

#include <game/state.hpp>

class Position {

    bool parent = false;
    float* parentX;
    float* parentY;
    float* parentWidth;
    float* parentHeight;
    float parentWidthRatio;
    float parentHeightRatio;

    SDL_FRect rect;

    float x;
    float y;
    float width;
    float height;

    public:


    SDL_Color color = SDL_Color{161, 195, 69, 255};

    Position(float x,
             float y, 
             float width, 
             float height,
             float* parentX, 
             float* parentY, 
             float* parentWidth, 
             float* parentHeight             
             ) {
        
        //cout << "Position  x:" << x << ", y:" << y  << ", w:" << width  << ", h:" << height << "\n";
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->parentX = parentX;
        this->parentY = parentY;
        this->parentWidth = parentWidth;
        this->parentHeight = parentHeight;
        this->parentWidthRatio = *parentWidth / width; 
        this->parentHeightRatio = *parentHeight / height; 
        //cout << "Position created\n";
    }

    float getX() {
        return *parentX + (x * ((*parentWidth / width) / parentWidthRatio));
    }

    float getY() {
        return *parentY + (y * ((*parentHeight / height) / parentHeightRatio));
    }

    float getWidth() {
        return width * ((*parentWidth / width) / parentWidthRatio);        
    }

    float getHeight() {
        return height * ((*parentHeight / height) / parentHeightRatio);        
    }

    SDL_FRect* getSDL_FRect() {
        rect.x = getX();
        rect.y = getY();
        rect.w = getWidth();
        rect.h = getHeight();
        return &rect;
    }

    void draw(State* state) {
        if (color.a>0){
            SDL_SetRenderDrawColor(state->renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawRectF(state->renderer, state->camera->translate(getSDL_FRect()));
            SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 0);
        }
    }


};

#endif
