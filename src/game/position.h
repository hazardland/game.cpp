// File: position.h

#ifndef GAME_POSITION_H
#define GAME_POSITION_H

#include <iostream>
#include <SDL2/SDL_image.h>

#include <game/state.h>

/**
 * @class Position
 * @brief Class for representing and manipulating a position in a 2D space.
 *
 * Position contains x and y coordinates, width, height, and related operations.
 * It also handles parent-child relationships between objects.
 */
class Position {
public:
    Position(float x, float y, float width, float height,
             float* parentX, float* parentY,
             float* parentWidth, float* parentHeight);
    ~Position() = default; // Add a virtual destructor

    float getX();
    float getY();
    float getWidth();
    float getHeight();
    SDL_FRect* getSDL_FRect();
    void draw(State* state);

private:
    bool parent = false;
    float* parentX;
    float* parentY;
    float* parentWidth;
    float* parentHeight;
    float parentWidthRatio;
    float parentHeightRatio;
    SDL_FRect rect;
    float x;
    float y;
    float width;
    float height;
    SDL_Color color = SDL_Color{161, 195, 69, 255};
};

#endif // GAME_POSITION_H
