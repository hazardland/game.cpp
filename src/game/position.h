// File: position.h

#ifndef GAME_POSITION_H
#define GAME_POSITION_H

#include <SDL3_image/SDL_image.h>
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

    Position(float x, float y, float width, float height,
             float* parentX, float* parentY);

    Position(float x, float y, float width, float height);
    Position(float width, float height);
    Position();

    ~Position() = default; // Add a virtual destructor

    float getX();
    float getY();
    float getWidth();
    float getHeight();
    void addPosition(float x, float y);
    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setX(float x);
    void setY(float y);
    void setHeight(float height);
    void setWidth(float width);

    void setRequiresUpdate();  // Mark that recalculation is needed
    SDL_FRect* getPosition();
    void draw(State* state);
    bool isReady();

    SDL_FRect rect;

private:
    void recalculateIfNeeded();
    bool parent = false;
    bool needsUpdate = true;
    float* parentX;
    float* parentY;
    float* parentWidth;
    float* parentHeight;
    float parentWidthRatio;
    float parentHeightRatio;
    float x;
    float y;
    float width;
    float height;
    SDL_Color color = SDL_Color{161, 195, 69, 255};
    bool ready = false;
};

#endif // GAME_POSITION_H
