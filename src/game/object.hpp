#ifndef GAME_OBJECT
#define GAME_OBJECT

using namespace std;
#include <iostream>

#include <SDL2/SDL_image.h>

#include <game/state.hpp>
#include <game/position.hpp>
#include <game/scene.hpp>

/*
    This methods work here for actual game world position and size
    The collision is calculated by object's x,y,width,height this dimenssions
    And the object viewRewct is equal to objects position
    If you want game world position and size was different than on screen
    (For example different hitbox then frame size) than use Unit class
    Which has hitbox and viewbox additionally 

    The id is allocated only when the id is requested
*/
class Object {


        int id = 0;

        int layer = 0;

        SDL_FRect position;

    public:

        static int count;    

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

        virtual void addPosition(float x, float y) {
            position.x += x;
            position.y += y;
        }

        virtual void setPosition(float x, float y) {
            position.x = x;
            position.y = y;
        }

        virtual void setSize(float width, float height) {
            position.w = width;
            position.h = height;
        }

        virtual void setX(float x) {
            position.x = x;
        }

        virtual void setY(float y) {
            position.y = y;
        }

        virtual void setHeight(float height) {
            position.h = height;
        }

        virtual void setWidth(float width) {
            position.w = width;
        }

        virtual float getX() {
            return position.x;
        }

        virtual float getY() {
            return position.y;
        }

        virtual float getHeight() {
            return position.h;
        }

        virtual float getWidth() {
            return position.w;
        }

        virtual int getLayer () {
            return layer;
        }

        virtual void setLayer (int layer) {
            this->layer = layer;
        }

        virtual SDL_FRect* getPosition () {
            return &position;
        }

        virtual SDL_FRect* getRenderPosition () {
            return &position;
        }

        virtual Position* createPosition(float x, float y, float width, float height) {
            // cout << "Creating position\n";
            return new Position(x, y, width, height, &position.x, &position.y, &position.w, &position.h);
        }

        virtual void drawPosition (State* state) {
            SDL_SetRenderDrawColor(state->renderer, 161, 195, 69, 255);
            SDL_RenderDrawRectF(state->renderer, state->camera->translate(getPosition()));
            SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 0);
        }

        virtual bool isVisible (State* state) {
            return state->camera->isVisible(getRenderPosition());            
        }

        virtual bool hasMinimap() {
            return false;
        }

        virtual Uint32 getMinimapColor(SDL_PixelFormat* format) {
            return 0;
        }

        // virtual SDL_Rect getMinimapRect() {
        //     return {0,0,0,0};
        // }

        virtual void update(State* state) {

        }

        virtual void render(State* state) {

        }

};

int Object::count = 0;

#endif
