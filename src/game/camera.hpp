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
    SDL_Rect result;
    int x=0;
    int y=0;
    int width;
    int height;
    virtual bool isVisible(SDL_Rect* position) {
        if (position->x + position->w  > x && 
            position->y + position->h > y &&
            position->x < x + width && 
            position->y < y + height
        ) {
            return true;
        }
        return false;
    }
    // virtual bool isVisible(int x, int y, SDL_Rect* position) {
    //     if (x + position->w  > this->x && 
    //         y + position->h > this->y &&
    //         x < this->x + this->width && 
    //         y < this->y + this->height
    //     ) {
    //         return true;
    //     }
    //     return false;
    // }
    virtual SDL_Rect* translate(SDL_Rect* position) {
        result.x = position->x - x; 
        result.y = position->y - y;
        result.w = position->w;
        result.h = position->h;
        return &result; 
    }
    // virtual void translate(int x, int y, SDL_Rect* position) {
    //     position->x = x - this->x; 
    //     position->y = y - this->y; 
    // }

};

#endif