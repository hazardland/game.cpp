#include <game/animation.h>

#include <iostream>

#include <game/clip.h>
#include <game/sprite.h>
#include <game/image.h>
#include <game/frame.h>

Animation::Animation(Sprite* sprite, int defaultClipName) {
    this->sprite = sprite;
    this->image = sprite->image;
    this->play(defaultClipName);
}

void Animation::render(SDL_FRect* position) {
    image->render(clip->getFrame(frame)->getRect(), position, clip->getFrame(frame)->getFlip());
}

void Animation::play(int clipName, float slow) {
    if (clipName == activeClip) {
        return;
    }

    auto clipIter = sprite->clips.find(clipName);
    if (clipIter == sprite->clips.end()) {
        std::cout << "Unknown clip " << clipName << "\n";
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

void Animation::update(uint32_t delta) {
    timer += delta;
    if (timer < pause){
        return;
    }
    timer = 0;

    frame = (frame + 1) % clip->getFrameCount();
}
