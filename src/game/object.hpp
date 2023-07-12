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
    Have your own float x, float y to track object's game world position 
*/
class Object {

        SDL_FRect position;
        
        // SDL_FRect output;
        // SDL_FRect hitbox;
    
    public:
        
        static int count;    

        int layer = -1;

        // To use this position as absolute position in conjuction with camera
        // Use ->render(camera->translate(object->position)) instead
        // But if you want to use translated position further in rendering
        // User SDL_Rect camera->result after camera->translate()

        // float x;
        // float y;
        
        int id = 0;

        void setId() {
            if (id==0) {
                Object::count++;
                id = Object::count;
            }
        }

        int getId() {
            setId();
            return id;
        }

        virtual void update(State* state) {

        }
        virtual void render(State* state) {

        }
        virtual Object* setPosition (float x, float y) {
            position.x = x;
            position.y = y;
            return this;
        }
        virtual SDL_FRect* getPosition() {
            return &position;
        }
        virtual void addPosition (float x, float y) {
            position.x += x; 
            position.y += y; 
        }
        virtual void setSize (float width, float height) {
            position.w = width;
            position.h = height;
        }
        virtual void setX (float x) {
            position.x = x;
        }
        virtual float getX() {
            return position.x;
        }
        virtual void setY (float y) {
            position.y = y;
        }
        virtual float getY() {
            return position.y;
        }        
        virtual void setWidth (float width) {
            position.w = width;
        }
        virtual float getWidth() {
            return position.w;
        }
        virtual void setHeight (float height) {
            position.h = height;
        }
        virtual float getHeight() {
            return position.h;
        }
        virtual bool hasMinimap() {
            return false;
        }
        virtual Uint32 getMinimapColor(SDL_PixelFormat* format) {
            return 0;
        }
        virtual SDL_Rect getMinimapRect() {
            return {0,0,0,0};
        }

};

int Object::count = 0;

#endif
