// File: game/fps.cpp
#include "game/fps.h"

#include <SDL3/SDL_timer.h>

#include <game/state.h>
#include <game/clock.h>
#include <game/camera.h>
#include <game/screen.h>

// Fps::Fps() {
//     cooldown = new Cooldown(200);
// }

void Fps::update(State* state) {
        if (cooldown->isReady()) {
            setText(std::to_string(state->clock->fps) + " FPS / DELTA " + std::to_string(state->clock->delta));
            setX(static_cast<float>(state->screen->getWidth() - getWidth() - 5));
            cooldown->reset();
        }

}

bool Fps::isVisible(State* /* state */) {
    return true;
}
