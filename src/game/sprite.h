#ifndef GAME_SPRITE_H
#define GAME_SPRITE_H

using namespace std;

#include <map>
#include <string>
#include <stdio.h>

#include <SDL2/SDL_image.h>

#include <game/clip.h>
#include <game/image.h>

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
    void add(int clipName,
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
        // cout << "Adding clip " << clipName << " " << clips.size() << "\n";
    }
    ~Sprite() {
        delete image;
    }
};
#endif
