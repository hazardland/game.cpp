#ifndef GAME_FPS
#define GAME_FPS

#include <game/text.hpp>
#include <SDL2/SDL_timer.h>

class Fps: public Text {
    using Text::Text;
    public:
    int ticks = 0;
    virtual void update(State* state) {
        if (SDL_GetTicks()-ticks>500) {
            setText(to_string(state->clock->fps) + " FPS");
            setX((float)state->camera->width - getWidth()-5);
            ticks = SDL_GetTicks();
        }
    }

};

#endif
