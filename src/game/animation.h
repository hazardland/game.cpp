#ifndef GAME_ANIMATION_H
#define GAME_ANIMATION_H

#include <SDL2/SDL_image.h>

class Clip;
class Frame;
class Sprite;
class Image;

/**
 * @class Animation
 * @brief A class that tracks animation and uses sprite to render selected clip.
 *
 * This class handles animation for sprites. It keeps track of the current frame
 * and clip, and updates the animation based on the elapsed time.
 */
class Animation {
    public:
    int pause = 0;    ///< Pause between frames, in milliseconds
    int timer = 0;    ///< Timer keeping track of the time since the last frame change
    int frame = 0;    ///< Current frame of the animation

    Sprite* sprite;   ///< Sprite used for the animation
    Clip* clip;       ///< Current clip of the animation
    Image* image;     ///< Image used in the animation

    int activeClip = 0;   ///< ID of the active clip

    /**
     * @brief Constructs a new Animation instance.
     * @param sprite Sprite used for the animation.
     * @param defaultClipName ID of the default clip to use for the animation.
     */
    Animation(Sprite* sprite, int defaultClipName);

    /**
     * @brief Renders the current frame of the animation.
     * @param position Where to render the animation.
     */
    virtual void render(SDL_FRect* position);

    /**
     * @brief Changes the active clip of the animation.
     * @param clipName ID of the clip to play.
     * @param slow Slowdown factor for the animation. If >0, the animation is slowed down by this factor.
     */
    virtual void play(int clipName, float speed=0);

    /**
     * @brief Updates the animation based on the time since the last update.
     * @param delta Time since the last update, in milliseconds.
     */
    virtual void update(uint32_t delta);
};

#endif
