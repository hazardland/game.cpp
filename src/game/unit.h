#ifndef GAME_UNIT_H
#define GAME_UNIT_H

using namespace std;
#include <iostream>

#include <vector>

#include <SDL2/SDL_image.h>

#include <game/input.h>

class Unit {
    protected:
        SDL_Rect position;

    public:

        virtual void update(uint32_t delta, Input* input) {

        }
        virtual void render(SDL_Renderer* renderer) {

        }

        void setPosition (int x, int y) {
            position.x = x;
            position.y = y;
        }
        void setSize (int width, int height) {
            position.w = width;
            position.h = height;
        }
        virtual int getWidth() {
            return position.w;
        }
        virtual int getHeight() {
            return position.h;
        }
};

#endif
