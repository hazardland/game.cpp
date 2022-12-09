#ifndef GAME_SPRITE
#define GAME_SPRITE

using namespace std;

#include <map>
#include <string>
#include <stdio.h>

#include <SDL2/SDL_image.h>

#include <game/clip.hpp>
#include <game/image.hpp>

/**
 * @brief Stores clips grouping frames and texture image
 * 
 */
class Sprite {
    public:
    int frameWidth;
    int frameHeight;
    int framePause;
    bool readVertically;
    Image* image; // Default image
    map<int, Clip*> clips;
    Sprite (Image* image,
            int frameWidth,
            int frameHeight,
            int framePause,
            bool readVertically = false
            ) {
        this->frameWidth = frameWidth;
        this->frameHeight = frameHeight;
        this->framePause = framePause;
        this->image = image;
        this->readVertically = readVertically;
    }
    void addClip(int clipName,
                 int startRow,
                 int startCell,
                 int frameCount,
                 bool flipX=false,
                 bool flipY=false) {
        clips[clipName] = new Clip(image, 
                                   frameWidth, 
                                   frameHeight, 
                                   startRow, startCell, 
                                   frameCount, 
                                   framePause, 
                                   flipX, flipY, readVertically);
    }
    ~Sprite() {
        delete image;
    }
};
#endif
