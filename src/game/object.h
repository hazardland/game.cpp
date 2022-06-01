#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

using namespace std;
#include <iostream>

#include <vector>

#include <SDL2/SDL_image.h>
#include <game/input.h>

class Object {
    protected:

        SDL_Rect position;

    public:

        virtual void update(uint32_t delta, Input* input) {

        }
        virtual void render() {

        }

        void setPosition (int x, int y) {
            position.x = x;
            position.y = y;
        }
        void setSize (int width, int height) {
            position.w = width;
            position.h = height;
        }
        virtual int getX() {
            return position.y;
        }        
        virtual int getY() {
            return position.y;
        }
        virtual int getWidth() {
            return position.w;
        }
        virtual int getHeight() {
            return position.h;
        }

};

#endif
