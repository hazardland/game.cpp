// File: state.cpp

#include "game/state.h"


#include <game/camera.h>
#include <game/clock.h>
#include <game/event.h>


State::State() {
    clock = new Clock();
    event = new Event();    
    camera = new Camera();    
}

State::~State() {
    delete clock;
    delete event;
    delete camera;
    // SDL_DestroyRenderer is usually called elsewhere, it's not called here to prevent double deletion
}
