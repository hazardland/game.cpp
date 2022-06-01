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
    Input input;
    const Uint8* keys;

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
        input.reset();

        //SDL_SetWindowTitle(window, to_string(clock.avgFps).c_str());

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
                    keys = SDL_GetKeyboardState(nullptr);
                    if (keys[SDL_SCANCODE_UP]) {
                        input.up = true;
                    }
                    if (keys[SDL_SCANCODE_DOWN]) {
                        input.down = true;
                    }
                    if (keys[SDL_SCANCODE_RIGHT]) {
                        input.right = true;
                    }
                    if (keys[SDL_SCANCODE_LEFT]) {
                        input.left = true;
                    }
                    if (keys[SDL_SCANCODE_SPACE]) {
                        input.space = true;
                    }
                    break;
                default:
                    break;
            }

        }

        scene->update(&clock, &input);
        scene->render();
        //SDL_Delay(1000/60);

    }

    return 0;

}
