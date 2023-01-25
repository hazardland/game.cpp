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
            int framePause = 60,
            bool readVertically = false
            ) {
        this->frameWidth = frameWidth;
        this->frameHeight = frameHeight;
        this->framePause = framePause;
        this->image = image;
        this->readVertically = readVertically;
    }
    Sprite* addClip(int clipId=1,
                 int startRow=1,
                 int startCell=1,
                 int frameCount=1,
                 bool flipX=false,
                 bool flipY=false) {
        clips[clipId] = new Clip(image, 
                                   frameWidth, 
                                   frameHeight, 
                                   startRow, startCell, 
                                   frameCount, 
                                   framePause, 
                                   flipX, flipY, readVertically);
        return this;
    }
    Clip* getClip(int clipId) {
        return clips[clipId];
    }
    SDL_Rect* getRect(int clipId=1, int frameIndex=0) {
        return getClip(clipId)->getFrame(frameIndex)->getRect();
    }
    ~Sprite() {
        delete image;
    }
};
#endif
