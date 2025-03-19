#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include <SDL2/SDL.h>

#include "game/window.h"
#include "game/keyboard.h"
#include "game/mouse.h"

/**
 * @class Input
 * Represents the current state of the system's events.
 */
class Input {
    public:
        bool close = false;  ///< Boolean representing if the window close event has been triggered.
        Keyboard* keyboard = new Keyboard();  ///< Keyboard object representing the current state of the keyboard.
        Mouse* mouse = new Mouse();  ///< Mouse object representing the current state of the mouse.
        // Screen* screen;  ///< Screen object for window-related operations.
        // void setScreen(Screen* screen);  ///< Sets the Screen object.
        void setWindow(Window* window);  ///< Sets the Screen object.
        void fetch();  ///< Fetches the current state of the system's events.
        ~Input();  ///< Destructor to properly delete resources.
    private:
        Window* window;  ///< Screen object for window-related operations.

    };

#endif
