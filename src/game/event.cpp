// File: game/event.cpp
#include "game/event.h"

#include <iostream>

#include <game/window.h>
// #include <game/screen.h>

void Keyboard::reset() {
    up = down = left = right = space = plus = minus = false;
}

void Mouse::reset() {
    rightClick = leftClick =  false;
}

bool Mouse::inside(SDL_FRect* position) {
    if (x>=position->x &&
        y>=position->y &&
        x<=position->x+position->w && 
        y<=position->y+position->h) {
            return true;
        } 
    return false;
}

// void Event::setScreen(Screen* screen) {
//     this->screen = screen;
// }

void Event::setWindow(Window* window) {
    this->window = window;
}


void Event::fetch() {
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
        // So basically if mause was down startX becomes>-1
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
                            window->onResize(event.window.data1, event.window.data2);
                            //SDL_Log("Window %d resized to %dx%d",
                            //        event.window.windowID, event.window.data1, event.window.data2);
                        break;
                    }
                    break;
                // case SDL_KEYDOWN:
                //     keyboard->keys = SDL_GetKeyboardState(nullptr);
                //     cout << "some key is down\n";
                //     if (keyboard->keys[SDL_SCANCODE_UP]) {
                //         cout << ">>>>> up is pressed\n";
                //         keyboard->up = true;
                //     }
                //     if (keyboard->keys[SDL_SCANCODE_DOWN]) {
                //         keyboard->down = true;
                //     }
                //     if (keyboard->keys[SDL_SCANCODE_RIGHT]) {
                //         keyboard->right = true;
                //     }
                //     if (keyboard->keys[SDL_SCANCODE_LEFT]) {
                //         keyboard->left = true;
                //     }
                //     if (keyboard->keys[SDL_SCANCODE_SPACE]) {
                //         keyboard->space = true;
                //     }
                //     break;
                // case SDL_KEYUP:
                //     cout << "some key is up\n";
                //     keyboard->keys = SDL_GetKeyboardState(nullptr);
                //     if (keyboard->keys[SDL_SCANCODE_UP]) {
                //         cout << "<<<<< up is released\n";
                //         keyboard->up = false;
                //     }
                //     if (keyboard->keys[SDL_SCANCODE_DOWN]) {
                //         keyboard->down = false;
                //     }
                //     if (keyboard->keys[SDL_SCANCODE_RIGHT]) {
                //         keyboard->right = false;
                //     }
                //     if (keyboard->keys[SDL_SCANCODE_LEFT]) {
                //         keyboard->left = false;
                //     }
                //     if (keyboard->keys[SDL_SCANCODE_SPACE]) {
                //         keyboard->space = false;
                //     }
                //     break;
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
   
    // update keyboard state here
    keyboard->keys = SDL_GetKeyboardState(nullptr);
    if (keyboard->keys[SDL_SCANCODE_UP]) {
        keyboard->up = true;
    } else {
        keyboard->up = false;
    }
    if (keyboard->keys[SDL_SCANCODE_DOWN]) {
        keyboard->down = true;
    } else {
        keyboard->down = false;
    }
    if (keyboard->keys[SDL_SCANCODE_RIGHT]) {
        keyboard->right = true;
    } else {
        keyboard->right = false;
    }
    if (keyboard->keys[SDL_SCANCODE_LEFT]) {
        keyboard->left = true;
    } else {
        keyboard->left = false;
    }
    if (keyboard->keys[SDL_SCANCODE_SPACE]) {
        keyboard->space = true;
    } else {
        keyboard->space = false;
    }
    if (keyboard->keys[SDL_SCANCODE_EQUALS]) {
        keyboard->plus = true;
    } else {
        keyboard->plus = false;
    }    
    if (keyboard->keys[SDL_SCANCODE_MINUS]) {
        keyboard->minus = true;
    } else {
        keyboard->minus = false;
    }    

}

Event::~Event() {
    delete keyboard;
    delete mouse;
}