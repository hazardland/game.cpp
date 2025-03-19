// File: state.cpp

#include "game/state.h"


#include <game/camera.h>
#include <game/clock.h>
#include <game/input.h>
#include <game/screen.h>


State::State() {
    clock = new Clock();
    input = new Input();
    camera = new Camera();
    screen = new Screen();
}

State::~State() {
    delete clock;
    delete input;
    delete camera;
    delete screen;
    // SDL_DestroyRenderer is usually called elsewhere, it's not called here to prevent double deletion
}
