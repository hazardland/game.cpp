// File: game/event.h

#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <SDL2/SDL.h>

// class Screen;
class Window;

/**
 * @class Keyboard
 * Represents a keyboard state at a certain moment.
 */
class Keyboard {
public:
    bool up = false;  ///< Boolean representing if 'up' key is pressed.
    bool down = false;  ///< Boolean representing if 'down' key is pressed.
    bool left = false;  ///< Boolean representing if 'left' key is pressed.
    bool right = false;  ///< Boolean representing if 'right' key is pressed.
    bool space = false;  ///< Boolean representing if 'space' key is pressed.
    bool plus = false;  ///< Boolean representing if 'space' key is pressed.
    bool minus = false;  ///< Boolean representing if 'space' key is pressed.
    const Uint8* keys;  ///< SDL key state array for checking specific keys.
    void reset();  ///< Resets the state of the keys.
};

/**
 * @class Mouse
 * Represents a mouse state at a certain moment.
 */
class Mouse {
public:
    int x;  ///< Mouse X position
    int y;  ///< Mouse Y position
    bool rightDragActive = false;  ///< Boolean representing if right drag is active.
    bool rightDragEnded = false;  ///< Boolean representing if right drag has ended.
    int rightDragStartX = -1;  ///< X position where right drag started.
    int rightDragStartY = -1;  ///< Y position where right drag started.
    bool rightClick = false;  ///< Boolean representing if right click is active.
    bool leftDragActive = false;  ///< Boolean representing if left drag is active.
    bool leftDragEnded = false;  ///< Boolean representing if left drag has ended.
    int leftDragStartX = -1;  ///< X position where left drag started.
    int leftDragStartY = -1;  ///< Y position where left drag started.
    bool leftClick = false;  ///< Boolean representing if left click is active.
    void reset();  ///< Resets the state of the mouse.
    bool inside(SDL_FRect* position);  ///< Checks if the mouse is inside a certain area defined by a SDL_FRect.
};

/**
 * @class Event
 * Represents the current state of the system's events.
 */
class Event {
public:
    bool close = false;  ///< Boolean representing if the window close event has been triggered.
    Keyboard* keyboard = new Keyboard();  ///< Keyboard object representing the current state of the keyboard.
    Mouse* mouse = new Mouse();  ///< Mouse object representing the current state of the mouse.
    // Screen* screen;  ///< Screen object for window-related operations.
    Window* window;  ///< Screen object for window-related operations.
    // void setScreen(Screen* screen);  ///< Sets the Screen object.
    void setWindow(Window* window);  ///< Sets the Screen object.
    void fetch();  ///< Fetches the current state of the system's events.
    ~Event();  ///< Destructor to properly delete resources.
};

#endif // GAME_EVENT_H
