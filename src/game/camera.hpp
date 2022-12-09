#ifndef GAME_CAMERA
#define GAME_CAMERA

#include <SDL2/SDL_image.h>

/**
 * @brief Camera tracking object
 * 
 */
class Camera
{
    public:
    int x=100;
    int y=100;
    int width;
    int height;
    virtual bool isVisible(int x, int y, int width, int height) {
        if (x+width>this->x && y+height>this->y &&
            x<this->x+this->width && y<this->y+this->height
        ) {
            return true;
        }
        return false;
    }
    virtual bool isVisible(SDL_Rect* position) {
        if (position->x + position->w > this->x && 
            position->y + position->h > this->y &&
            position->x < this->x + this->width && 
            position->y < this->y + this->height
        ) {
            return true;
        }
        return false;
    }
    virtual void translate(int x, int y, SDL_Rect* position) {
        position->x = x - this->x; 
        position->y = y - this->y; 
    }
    virtual void translate(SDL_Rect* position) {
        position->x -= this->x; 
        position->y -= this->y; 
    }
};

#endif
