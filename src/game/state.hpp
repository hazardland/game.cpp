#ifndef GAME_STATE
#define GAME_STATE

#include <SDL2/SDL_image.h>

#include <game/clock.hpp>
#include <game/event.hpp>
#include <game/camera.hpp>

/**
 * @brief Manage state in one object
 * 
 */
class State
{
    public:

    Clock* clock;
    Event* event;
    Camera* camera;
    SDL_Renderer* renderer;

    State() {
        clock = new Clock();
        event = new Event();    
        camera = new Camera();    
    }

};

#endif
