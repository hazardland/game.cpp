#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <iostream>
#include <SDL2/SDL_image.h>

class State;
class Position;
class Scene;

/*
    Methods in this class work with the actual game world position and size.
    Collision is calculated by the object's x, y, width, and height dimensions.
    The object's viewRect is equal to the object's position.
    If you want the game world position and size to be different than on screen
    (for example, a different hitbox than frame size) then use the Unit class
    which has a separate hitbox and viewbox.

    The id is allocated only when the id is requested.
*/

class Object {
    private:
        int id = 0;   // Object ID
        int layer = 0;    // Layer where the object exists
        SDL_FRect position;   // Object's position and size

    public:
        static int count;    // Keep count of total objects

        // Setter and getter methods
        void setId();
        int getId();
        virtual void addPosition(float x, float y);
        virtual void setPosition(float x, float y);
        virtual void setSize(float width, float height);
        virtual void setX(float x);
        virtual void setY(float y);
        virtual void setHeight(float height);
        virtual void setWidth(float width);
        virtual float getX();
        virtual float getY();
        virtual float getHeight();
        virtual float getWidth();
        virtual int getLayer();
        virtual void setLayer(int layer);
        virtual SDL_FRect* getPosition();
        virtual SDL_FRect* getRenderPosition();
        virtual Position* createPosition(float x, float y, float width, float height);
        virtual void drawPosition(State* state);
        virtual bool isVisible(State* state);
        virtual bool hasMinimap();
        virtual Uint32 getMinimapColor(SDL_PixelFormat* format);
        virtual void update(State* state);
        virtual void render(State* state);
};

#endif // GAME_OBJECT
