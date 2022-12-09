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
#include <game/clock.hpp>
#include <game/input.hpp>
#include <game/scene.hpp>
#include <game/camera.hpp>


bool SDL_STARTED = false;

class Window: public Screen {

    public:

    // SDL_Window object
    SDL_Window *window;

    Scene* scene;

    Clock* clock;
    Input* input;
    Camera* camera = NULL;

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

    virtual void onResize(int width, int height) {
        this->scene->width = width;
        this->scene->height = height;
        if (this->camera!=NULL){
            this->camera->width = width;
            this->camera->height = height;
        }
    }

    // virtual void prepare () {

    // }

    int run () {

        scene->prepare();

        while(!input->close){
            clock->tick();
            input->update();
            scene->update(clock, input, camera);
            scene->render(camera);
        }

        return 0;
    }

};

#endif
