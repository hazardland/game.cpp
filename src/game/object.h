#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

using namespace std;
#include <iostream>
#include <vector>

#include <SDL2/SDL_image.h>

#include <game/input.h>
#include <game/camera.h>

class Object {
    protected:
        // Game world position
        // Do we need actual position at all?
        // During render it should be decided 
        // where it should be rendered
        // Or if it should be rendered at all
        // int x;
        // int y;
        // Render position (Camera and game world position might differ)
        SDL_Rect position;

    public:

        virtual void update(uint32_t delta, Input* input) {

        }
        // The render method should decide to render or not and where to render
        // By updating position
        virtual void render(Camera* camera) {

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
