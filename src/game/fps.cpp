// File: game/fps.cpp
#include "game/fps.h"

#include <SDL2/SDL_timer.h>

#include <game/state.h>
#include <game/clock.h>
#include <game/camera.h>

void Fps::update(State* state) {
    if (SDL_GetTicks() - ticks > 500) {
        setText(std::to_string(state->clock->fps) + " FPS / DELTA " + std::to_string(state->clock->delta));
        setX(static_cast<float>(state->camera->width - getWidth() - 5));
        ticks = SDL_GetTicks();
    }
}

bool Fps::isVisible(State* /* state */) {
    return true;
}
