#ifndef GAME_OBJECT
#define GAME_OBJECT

using namespace std;

#include <iostream>
#include <vector>

#include <SDL2/SDL_image.h>

#include <game/state.hpp>
#include <game/camera.hpp>

/*
    All methods here work only for on screen position
    Have your own int x, int y to track object's game world position 
*/
class Object {

    public:

        int layer = -1;

        // To use this position as absolute position in conjuction with camera
        // Use ->render(camera->translate(object->position)) instead
        // But if you want to use translated position further in rendering
        // User SDL_Rect camera->result after camera->translate()
        SDL_Rect position;

        // int x;
        // int y;

        virtual void update(State* state) {

        }
        virtual void render(State* state) {

        }
        virtual void setPosition (int x, int y) {
            position.x = x;
            position.y = y;
        }
        virtual void setSize (int width, int height) {
            position.w = width;
            position.h = height;
        }
        virtual void setX (int x) {
            position.x = x;
        }
        virtual int getX() {
            return position.x;
        }
        virtual void setY (int y) {
            position.y = y;
        }
        virtual int getY() {
            return position.y;
        }        
        virtual void setWidth (int width) {
            position.w = width;
        }
        virtual int getWidth() {
            return position.w;
        }
        virtual void setHeight (int height) {
            position.h = height;
        }
        virtual int getHeight() {
            return position.h;
        }

};

#endif
