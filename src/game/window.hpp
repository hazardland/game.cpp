#ifndef GAME_WINDOW
#define GAME_WINDOW

using namespace std;

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include <game/screen.hpp>
// #include <game/clock.hpp>
// #include <game/event.hpp>
#include <game/scene.hpp>
#include <game/state.hpp>


bool SDL_STARTED = false;

class Window: public Screen {

    public:

    // SDL_Window object
    SDL_Window *window;

    Scene* scene;

    // Clock* clock;
    // Input* input;
    // Camera* camera = NULL;
    State* state;

    Window(const char* title,
           const int width,
           const int height,
           State* state = NULL) {

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
            printf("Failed to create window: %s", SDL_GetError());
        }

        // SDL_SetWindowOpacity(window, 0.6);
        // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

        if (state==NULL) {
            this->state = new State();
        }
        this->state->event->setScreen(this);
        printf("preping");
        SDL_GetWindowSize(window, &this->state->camera->width, &this->state->camera->height);
        
    }

    void setScene(Scene* scene) {
        state->renderer = scene->renderer;
        this->scene = scene;
        SDL_GetWindowSize(window, &scene->width, &scene->height);
    }

    // void setState(State* state) {
    //     this->state->event->setScreen(this);
    //     SDL_GetWindowSize(window, &camera->width, &camera->height);
    // }

    virtual void onResize(int width, int height) {
        scene->width = width;
        scene->height = height;
        state->camera->width = width;
        state->camera->height = height;
    }

    // virtual void prepare () {

    // }

    int run () {

        scene->prepare();

        while(!state->event->close){
            state->clock->tick();
            state->event->fetch();
            scene->update(state);
            scene->render(state);
        }

        return 0;
    }

};

#endif
