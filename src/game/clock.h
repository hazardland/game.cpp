#ifndef GAME_CLOCK_H
#define GAME_CLOCK_H

#include <SDL2/SDL_timer.h>

/**
 * @brief Simple clock for fps tracking
 * 
 */
class Clock
{
    public:

    uint32_t previousTicks = 0;
    uint32_t delta = 0;
    uint32_t fps = 0;
    uint32_t lowFps = -1;
    uint32_t highFps = 0;
    uint32_t avgFps = 0;

    void tick()
    {
        uint32_t ticks = SDL_GetTicks();
        delta = ticks - previousTicks;
        previousTicks = ticks;
        fps = 1000/delta;
        if (fps>highFps) {
            highFps = fps;
        }
        if (fps<lowFps && fps>10) {
            lowFps = fps;
        }
        avgFps = (avgFps + fps) / 2; 
    }
};

#endif
