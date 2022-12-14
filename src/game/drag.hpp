#ifndef GAME_DRAG
#define GAME_DRAG

#include <SDL2/SDL_image.h>

#include <game/state.hpp>

class Drag {
    public:
    bool dragging = false;
    bool dragLeft = true;
    bool dragRight = true;
    int dragStartX;
    int dragStartY;
    SDL_Rect* position;
    bool enabled = true;
    Drag(SDL_Rect* position, bool dragLeft, bool dragRight) {
        this->position = position;
        this->dragLeft = dragLeft;
        this->dragRight = dragRight;
    }
    void update(State* state) {
        if (enabled) {
            Mouse* mouse = state->event->mouse;
            if (dragRight && !dragging && mouse->rightDragActive && mouse->inside(position)) {
                dragging = true;
                dragStartX = mouse->x - position->x;
                dragStartY = mouse->y - position->y;
            }

            if (dragLeft && !dragging && mouse->leftDragActive && mouse->inside(position)) {
                dragging = true;
                dragStartX = mouse->x - position->x;
                dragStartY = mouse->y - position->y;
            }

            if (dragging) {
                position->x = mouse->x - dragStartX;
                position->y = mouse->y - dragStartY;
            }

            if (mouse->rightDragEnded && dragging) {
                dragging = false;
            }
        }
    }
    void disable() {
        dragging = false;
        enabled = false;
    }
    void enable() {
        enabled = true;
    }
};

#endif
