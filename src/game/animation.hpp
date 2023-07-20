#ifndef GAME_ANIMATION
#define GAME_ANIMATION

using namespace std;

#include <stdio.h>

#include <SDL2/SDL_image.h>

#include <game/sprite.hpp>
#include <game/image.hpp>
#include <game/clip.hpp>

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

    virtual void render(SDL_FRect* position) {
        image->render(clip->getFrame(frame)->getRect(), position, clip->getFrame(frame)->getFlip());
    }

    // virtual void play(int clipName, float slow=0) {
    //     if (clipName==activeClip) {
    //         return;
    //     }
    //     if (!sprite->clips.contains(clipName)) {
    //         cout<<"Unknown clip "<<clipName<<"\n";
    //     } else {
    //         // cout<<"Playing clip "<<activeClip<<"\n";
    //     }
    //     clip = sprite->clips[clipName];
    //     timer = 0;
    //     frame = 0;
    //     if (slow>0) {
    //         pause = clip->getFramePause()*slow;
    //     } else {
    //         pause = clip->getFramePause();
    //     }
    //     activeClip = clipName;
    //     // cout << "Set clip "<< clipName << " " << clip << "\n";
    // }

    virtual void play(int clipName, float slow=0) {
        if (clipName == activeClip) {
            return;
        }

        auto clipIter = sprite->clips.find(clipName);
        if (clipIter == sprite->clips.end()) {
            cout << "Unknown clip " << clipName << "\n";
            return;
        } 

        clip = clipIter->second;
        timer = 0;
        frame = 0;
        if (slow > 0) {
            pause = clip->getFramePause() * slow;
        } else {
            pause = clip->getFramePause();
        }
        activeClip = clipName;
    }


    // virtual void update(uint32_t delta) {
    //     // cout << "Updating";
    //     timer += delta;
    //     if (timer < pause){
    //         return;
    //     }
    //     timer = 0;

    //     frame += 1;
    //     if (frame == clip->getFrameCount()) {
    //         frame = 0;
    //     }
    //     // cout << "Frame: " <<frame<<"\n";
    // }

    virtual void update(uint32_t delta) {
        timer += delta;
        if (timer < pause){
            return;
        }
        timer = 0;

        frame = (frame + 1) % clip->getFrameCount();
    }

};
#endif
