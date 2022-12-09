#ifndef GAME_INPUT
#define GAME_INPUT

using namespace std;

#include <iostream>

#include <SDL2/SDL.h>

#include <game/screen.hpp>

class Keyboard {
    public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool space = false;
    const Uint8* keys;
    void reset() {
        up = down = left = right = space = false;
    }
};

class Mouse {
    public:
    
    int x;
    int y;

    bool rightDragActive = false;
    bool rightDragEnded = false;
    int rightDragStartX = -1;
    int rightDragStartY = -1;
    bool rightClick = false;
   
    bool leftDragActive = false;   
    bool leftDragEnded = false;
    int leftDragStartX = -1;
    int leftDragStartY = -1;
    bool leftClick = false;


    void reset() {
        rightClick = leftClick =  false;
    }

};

class Input {
    public:
    bool close = false;
    Keyboard* keyboard = new Keyboard();
    Mouse* mouse = new Mouse();
    Screen* screen;

    void setWindow(Screen* screen) {
        this->screen = screen;
    }
    

    void update() {
        keyboard->reset();
        mouse->reset();

        //SDL_SetWindowTitle(window, to_string(clock.avgFps).c_str());
        SDL_GetMouseState(&mouse->x, &mouse->y);

        // If drag ended in previous tick we reset drag
        if (mouse->rightDragEnded) {
            mouse->rightDragEnded = false;
            mouse->rightDragStartX = -1;
            mouse->rightDragStartY = -1;
            // cout << "Cleaning right drag\n";
        // So basically if muose was down startX becomes>-1
        // If startX != x means we moved
        // So it can be treated as drag start
        } else if (!mouse->rightDragActive
                   && mouse->rightDragStartX>-1
                   && (mouse->rightDragStartX!=mouse->x
                       || mouse->rightDragStartY!=mouse->y)) {
            mouse->rightDragActive = true;
            // cout << "Activating right drag\n";
        }

        if (mouse->leftDragEnded) {
            mouse->leftDragEnded = false;
            mouse->leftDragStartX = -1;
            mouse->leftDragStartY = -1;
            // cout << "Cleaning left drag\n";
        } else if (!mouse->leftDragActive
                   && mouse->leftDragStartX>-1
                   && (mouse->leftDragStartX!=mouse->x
                       || mouse->leftDragStartY!=mouse->y)) {
            mouse->leftDragActive = true;
            // cout << "Activating left drag\n";
        }

        SDL_Event event;
        while(SDL_PollEvent(&event)){

            switch(event.type){
                case SDL_QUIT:
                    close = true;
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event){
                        case SDL_WINDOWEVENT_RESIZED:
                            // window->width = event.window.data1;
                            // window->height = event.window.data2;
                            screen->onResize(event.window.data1, event.window.data2);
                            //SDL_Log("Window %d resized to %dx%d",
                            //        event.window.windowID, event.window.data1, event.window.data2);
                        break;
                    }
                    break;
                case SDL_KEYDOWN:
                    keyboard->keys = SDL_GetKeyboardState(nullptr);
                    if (keyboard->keys[SDL_SCANCODE_UP]) {
                        keyboard->up = true;
                    }
                    if (keyboard->keys[SDL_SCANCODE_DOWN]) {
                        keyboard->down = true;
                    }
                    if (keyboard->keys[SDL_SCANCODE_RIGHT]) {
                        keyboard->right = true;
                    }
                    if (keyboard->keys[SDL_SCANCODE_LEFT]) {
                        keyboard->left = true;
                    }
                    if (keyboard->keys[SDL_SCANCODE_SPACE]) {
                        keyboard->space = true;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    // This could be potential drag start so save x, y
                    // Drag will only be activated on next tick
                    // If mouse moves to another position
                    // But before mouse is UP
                    if (event.button.button==SDL_BUTTON_RIGHT) {
                        mouse->rightDragStartX = mouse->x;
                        mouse->rightDragStartY = mouse->y;
                        // cout << "Right mouse down\n";
                    } else if (event.button.button==SDL_BUTTON_LEFT) {
                        mouse->leftDragStartX = mouse->x;
                        mouse->leftDragStartY = mouse->y;
                        // cout << "Left mouse down\n";
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button==SDL_BUTTON_RIGHT) {
                        // So if ever drag activated
                        // We can deactivate drag
                        if (mouse->rightDragActive) {
                            mouse->rightDragEnded = true;
                            mouse->rightDragActive = false;
                            // cout << "Right drag end\n";
                        // So if drag is not active
                        } else {
                            mouse->rightClick = true;
                            mouse->rightDragStartX = -1;
                            mouse->rightDragStartY = -1;
                            // cout << "Right click\n";
                        }
                    // I believe this event come separately
                    } else if (event.button.button==SDL_BUTTON_LEFT) {
                        if (mouse->leftDragActive) {
                            mouse->leftDragEnded = true;
                            mouse->leftDragActive = false;
                            // cout << "Left drag end\n";
                        } else {
                            mouse->leftClick = true;
                            mouse->leftDragStartX = -1;
                            mouse->leftDragStartY = -1;
                            // cout << "Left click\n";
                        }
                    }
                    // if any of drags active we just ended drag

                    break;

                default:
                    break;
            }

        }
    }

};

#endif
