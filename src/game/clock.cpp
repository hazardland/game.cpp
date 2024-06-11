#include "game/clock.h"

void Clock::tick() {
    uint32_t ticks = SDL_GetTicks();
    delta = ticks - previousTicks;
    previousTicks = ticks;
    fps = (delta > 0) ? 1000.0f / delta : 0.0f;
}
