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
    int width;
    int height;
    int pause;
    bool readVertically;
    Image* image; // Default image
    map<int, Clip*> clips;
    Sprite (Image* image,
            int width,
            int height,
            int pause,
            bool readVertically = false
            ) {
        this->width = width;
        this->height = height;
        this->pause = pause;
        this->image = image;
        this->readVertically = readVertically;
    }
    void add(int clipName,
                 int row,
                 int cell,
                 int count,
                 bool flipX=false,
                 bool flipY=false) {
        clips[clipName] = new Clip(image, width, height, row, cell, count, pause, flipX, flipY, readVertically);
        // cout << "Adding clip " << clipName << " " << clips.size() << "\n";
    }
    ~Sprite() {
        delete image;
    }
};
#endif
