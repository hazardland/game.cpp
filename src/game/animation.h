#ifndef GAME_ANIMATION_H
#define GAME_ANIMATION_H

using namespace std;
#include <stdio.h>

#include <SDL2/SDL_image.h>
#include <game/sprite.h>
#include <game/image.h>
#include <game/clip.h>

/**
 * @brief Tracks animation and uses sprite to render selected clip
 * 
 */
class Animation  {

    public:

    int pause = 0;
    int timer = 0;
    int frame = 0;

    Sprite* sprite;

    Clip* clip;
    Image* image;

    int activeClip = 0;

    Animation(Sprite* sprite, int defaultClipName) {
        // cout << "Creating animation\n";
        this->sprite = sprite;
        this->image = sprite->image;
        this->play(defaultClipName);
        // cout << "Default clip "<< defaultClip << " " << clip << "\n";
    }

    virtual void render(SDL_Rect* position) {
        image->render(clip->getFrame(frame)->getRect(), position, clip->getFrame(frame)->getFlip());
    }

    virtual void play(int clipName, float slow=0) {
        if (clipName==activeClip) {
            return;
        }
        clip = sprite->clips[clipName];
        if (slow>0) {
            pause = clip->getPause()*slow;
        } else {
            pause = clip->getPause();
        }
        activeClip = clipName;
        // cout << "Set clip "<< clipName << " " << clip << "\n";
    }

    virtual void update(uint32_t delta) {
        // cout << "Updating";
        timer += delta;
        if (timer < pause){
            return;
        }
        timer = 0;

        frame += 1;
        if (frame == clip->getFrameCount()) {
            frame = 0;
        }
    }

};
#endif
