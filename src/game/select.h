// File: select.h

#ifndef GAME_SELECT_H
#define GAME_SELECT_H

#include "game/rectangle.h"

class State;

/**
 * @brief The Select class extends from the Rectangle class and represents a selection area in the game.
 *
 * This class handles the update of the selection area according to the mouse position and actions.
 */
class Select: public Rectangle {
public:
    using Rectangle::Rectangle;

    // Function declaration
    void update(State* state) override;
};

#endif // GAME_SELECT_H
