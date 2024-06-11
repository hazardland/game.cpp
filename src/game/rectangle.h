// File: rectangle.h

#ifndef GAME_RECTANGLE_H
#define GAME_RECTANGLE_H

#include <SDL2/SDL.h>

#include "game/object.h"

class State;

/**
 * @brief The Rectangle class extends from the Object class and represents a rectangle in the game.
 *
 * This class handles the rendering of the rectangle and controls its visibility.
 */
class Rectangle: public Object {
public:
    SDL_Renderer* renderer;
    SDL_Color background;
    SDL_Color border;
    bool visible;

    Rectangle(SDL_Renderer* renderer, SDL_Color border = {}, SDL_Color background = {});
    void show();
    void hide();
    void render(State* state) override;
};

#endif // GAME_RECTANGLE_H
