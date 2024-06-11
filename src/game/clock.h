#ifndef GAME_CLOCK_H
#define GAME_CLOCK_H

#include <SDL2/SDL_timer.h>

/**
 * @brief Simple clock for fps tracking.
 * It calculates the number of frames per second (fps) and the time delta.
 */
class Clock {
    private:
        uint32_t previousTicks = 0;

    public:
        uint32_t delta = 0;    ///< Time difference in milliseconds between the current and the previous tick.
        uint32_t fps = 0;      ///< The number of frames per second.

        /**
         * @brief Updates delta and fps values based on the current tick.
         */
        void tick();
};

#endif // GAME_CLOCK_H
