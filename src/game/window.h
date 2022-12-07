#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

using namespace std;

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#include <game/view.h>
#include <game/clock.h>
#include <game/input.h>
#include <game/scene.h>
#include <game/camera.h>


bool SDL_STARTED = false;

class Window: public View {

    public:

    SDL_Window *window;

    Clock* clock;
    Scene* scene;
    Input* input;
    Camera* camera = NULL;

    bool closed = false;
    bool resized = false;
    int width;
    int height;


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
        this->input->setWindow(this);
    }

    void setCamera(Camera* camera) {
        this->camera = camera;
        SDL_GetWindowSize(window, &camera->width, &camera->height);
    }

    virtual void onResized(int width, int height) {

        printf("Resized\n");
        this->width = width;
        this->height = height;
        this->scene->width = this->width;
        this->scene->height = this->height;
        if (this->camera!=NULL){
            this->camera->width = this->width;
            this->camera->height = this->height;
        }
        printf("Done resize\n");

    }

    // virtual void prepare () {

    // }

    int run () {

        scene->prepare();

        while(!input->closed){
            clock->tick();
            input->update();
            scene->update(clock, input, camera);
            scene->render(camera);
        }

        return 0;
    }

};

#endif
