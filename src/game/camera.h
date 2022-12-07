#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include <SDL2/SDL_image.h>

/**
 * @brief Simple clock for fps tracking
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
    virtual void translate(int x, int y, SDL_Rect* position) {
        position->x = x - this->x; 
        position->y = y - this->y; 
    }
};

#endif
