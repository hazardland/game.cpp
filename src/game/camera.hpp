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
    int x=0;
    int y=0;
    int width;
    int height;
    // virtual bool isVisible(int x, int y, int width, int height) {
    //     if (x + width  > this->x && 
    //         y + height > this->y &&
    //         x < this->x + this->width && 
    //         y < this->y + this->height
    //     ) {
    //         return true;
    //     }
    //     return false;
    // }
    virtual bool isVisible(int x, int y, SDL_Rect* position) {
        if (x + position->w  > this->x && 
            y + position->h > this->y &&
            x < this->x + this->width && 
            y < this->y + this->height
        ) {
            return true;
        }
        return false;
    }
    /*
        Store int x and int y separately outside of SDL_rect position
        Will change position->x and position->y and will discard
        Your original position 
    */
    virtual void translate(int x, int y, SDL_Rect* position) {
        position->x = x - this->x; 
        position->y = y - this->y; 
    }
};

#endif
