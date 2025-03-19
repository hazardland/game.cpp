// File: state.h

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL2/SDL_image.h>

class Clock;
class Input;
class Camera;
class Screen;

/**
 * @class State
 * @brief Represents the state of the game and manages it in a single object.
 *
 * State class contains instances of Clock, Event, and Camera classes and
 * the SDL_Renderer object, which are all important components of the game state.
 */
class State {
public:
    State();
    ~State();  // Add a destructor

    Clock* clock;
    Input* input;
    Camera* camera;
    Screen* screen;
    SDL_Renderer* renderer;
};

#endif // GAME_STATE_H
