// File: select.cpp

#include "game/select.h"

#include "game/state.h"
#include "game/input.h"


void Select::update(State* state) {
    Mouse* mouse = state->input->mouse;
    if (mouse->leftDragActive) {
        show();
        if (mouse->x > mouse->leftDragStartX) {
            setX(mouse->leftDragStartX);
            setWidth(mouse->x - mouse->leftDragStartX);
        } else {
            setX(mouse->x);
            setWidth(mouse->leftDragStartX - mouse->x);
        }
        if (mouse->y > mouse->leftDragStartY) {
            setY(mouse->leftDragStartY);
            setHeight(mouse->y - mouse->leftDragStartY);
        } else {
            setY(mouse->y);
            setHeight(mouse->leftDragStartY - mouse->y);
        }
    } else {
        hide();
    }
}
