using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <iostream>

#include <game/clock.h>
#include <game/input.h>
#include <game/sprite.h>

#include <footman.h>
#include <enum.h>

#include <war2.h>


#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500


int main(int argc, char** argv){

    Clock clock;
    Input* input = new Input();

    if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
        printf("Failed to SDL: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init()==-1) {
        printf("Failed to TTF: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Game Title",
                                          SDL_WINDOWPOS_UNDEFINED, 
                                          SDL_WINDOWPOS_UNDEFINED, 
                                          SCREEN_WIDTH, 
                                          SCREEN_HEIGHT, 
                                          SDL_WINDOW_RESIZABLE);
    if(!window){
        printf("Failed to open window: %s", SDL_GetError());
        return 1;
    }


    Scene* scene = new Game(window, SCREEN_HEIGHT, SCREEN_HEIGHT);
    scene->prepare();

    bool running = true;
    while(running){
        clock.tick();

        input->keyboard->reset();
        input->mouse->reset();

        //SDL_SetWindowTitle(window, to_string(clock.avgFps).c_str());
        SDL_GetMouseState(&input->mouse->x, &input->mouse->y);
        
        // If drag ended in previous tick we reset drag
        if (input->mouse->rightDragEnded) {
            input->mouse->rightDragEnded = false;
            input->mouse->rightDragStartX = -1;
            input->mouse->rightDragStartY = -1;
            cout << "Cleaning right drag\n";
        // So basically if muose was down startX becomes>-1
        // If startX != x means we moved
        // So it can be treated as drag start
        } else if (!input->mouse->rightDragActive
                   && input->mouse->rightDragStartX>-1 
                   && (input->mouse->rightDragStartX!=input->mouse->x 
                       || input->mouse->rightDragStartY!=input->mouse->y)) {
            input->mouse->rightDragActive = true;
            cout << "Activating right drag\n";
        }

        if (input->mouse->leftDragEnded) {
            input->mouse->leftDragEnded = false;
            input->mouse->leftDragStartX = -1;
            input->mouse->leftDragStartY = -1;
            cout << "Cleaning left drag\n";
        } else if (!input->mouse->leftDragActive
                   && input->mouse->leftDragStartX>-1 
                   && (input->mouse->leftDragStartX!=input->mouse->x 
                       || input->mouse->leftDragStartY!=input->mouse->y)) {
            input->mouse->leftDragActive = true;
            cout << "Activating left drag\n";
        }


        SDL_Event event;
        while(SDL_PollEvent(&event)){

            switch(event.type){
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event){
                        case SDL_WINDOWEVENT_RESIZED:
                            scene->setSize(event.window.data1, event.window.data2);
                            SDL_Log("Window %d resized to %dx%d",
                                    event.window.windowID, event.window.data1,
                                    event.window.data2);
                        break;
                    }
                    break;
                case SDL_KEYDOWN:
                    input->keyboard->keys = SDL_GetKeyboardState(nullptr);
                    if (input->keyboard->keys[SDL_SCANCODE_UP]) {
                        input->keyboard->up = true;
                    }
                    if (input->keyboard->keys[SDL_SCANCODE_DOWN]) {
                        input->keyboard->down = true;
                    }
                    if (input->keyboard->keys[SDL_SCANCODE_RIGHT]) {
                        input->keyboard->right = true;
                    }
                    if (input->keyboard->keys[SDL_SCANCODE_LEFT]) {
                        input->keyboard->left = true;
                    }
                    if (input->keyboard->keys[SDL_SCANCODE_SPACE]) {
                        input->keyboard->space = true;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    // This could be potential drag start so save x, y
                    // Drag will only be activated on next tick
                    // If mouse moves to another position 
                    // But before mouse is UP
                    if (event.button.button==SDL_BUTTON_RIGHT) {
                        input->mouse->rightDragStartX = input->mouse->x;
                        input->mouse->rightDragStartY = input->mouse->y;
                        cout << "Right mouse down\n";
                    } else if (event.button.button==SDL_BUTTON_LEFT) {
                        input->mouse->leftDragStartX = input->mouse->x;
                        input->mouse->leftDragStartY = input->mouse->y;
                        cout << "Left mouse down\n";
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button==SDL_BUTTON_RIGHT) {
                        // So if ever drag activated
                        // We can deactivate drag
                        if (input->mouse->rightDragActive) {                   
                            input->mouse->rightDragEnded = true;
                            input->mouse->rightDragActive = false;
                            cout << "Right drag end\n";
                        // So if drag is not active
                        } else {
                            input->mouse->rightClick = true;
                            input->mouse->rightDragStartX = -1;
                            input->mouse->rightDragStartY = -1;
                            cout << "Right click\n";
                        }
                    // I believe this event come separately
                    } else if (event.button.button==SDL_BUTTON_LEFT) {
                        if (input->mouse->leftDragActive) {                   
                            input->mouse->leftDragEnded = true;
                            input->mouse->leftDragActive = false;
                            cout << "Left drag end\n";
                        } else {
                            input->mouse->leftClick = true;
                            input->mouse->leftDragStartX = -1;
                            input->mouse->leftDragStartY = -1;
                            cout << "Left click\n";
                        }
                    }
                    // if any of drags active we just ended drag 
            
                    break;

                default:
                    break;
            }

        }

        scene->update(&clock, input);
        scene->render();

        
        //SDL_Delay(1000/60);

    }

    return 0;

}
