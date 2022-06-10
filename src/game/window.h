#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

using namespace std;

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include <game/clock.h>
#include <game/input.h>
#include <game/scene.h>

bool SDL_STARTED = false;

class Window {

    public:

    SDL_Window *window;

    Clock* clock;
    Scene* scene;
    Input* input;

    Window(const char* title,
           const int width,
           const int height) {

        if (!SDL_STARTED) {

            if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
                printf("Failed to SDL: %s", SDL_GetError());
            }

            if (TTF_Init()==-1) {
                printf("Failed to TTF: %s", SDL_GetError());
            }

            SDL_STARTED = true;
        }
        window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  width,
                                  height,
                                  SDL_WINDOW_RESIZABLE);

        if(!window){
            printf("Failed to open window: %s", SDL_GetError());
        }

        clock = new Clock();

    }

    void setScene(Scene* scene) {
        this->scene = scene;
        SDL_GetWindowSize(window, &scene->width, &scene->height);
    }

    void setInput(Input* input) {
        this->input = input;
        input->width = &scene->width;
        input->height = &scene->height;
    }

    // virtual void prepare () {

    // }

    int run () {

        scene->prepare();

        while(!input->quit){
            clock->tick();
            input->update();
            scene->update(clock, input);
            scene->render();
        }

        return 0;
    }

};

#endif
