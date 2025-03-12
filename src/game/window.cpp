// File: window.cpp

#include <iostream>
// #include <SDL2/SDL.h>

#include "game/window.h"


#include "game/scene.h"
#include "game/state.h"
#include "game/state.h"
#include "game/event.h"
#include "game/clock.h"
#include "game/camera.h"
#include "game/screen.h"

bool SDL_STARTED = false;  // Define the global variable here.

Window::Window(const char* title, const int width, const int height, State* state) {

    if (!SDL_STARTED) {
        if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
            std::cout << "Failed to SDL: " << SDL_GetError() << std::endl;
        }

        if (TTF_Init()==-1) {
            std::cout << "Failed to TTF: " << SDL_GetError() << std::endl;
        }

        SDL_STARTED = true;
    }

    std::cout << "Window size " << width << "x" << height << "\n";

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);

    if(!window){
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
    }

    if (!state) {
        std::cout << "Creating state\n";
        this->state = new State();
    }
    this->state->event->setWindow(this);
    SDL_GetWindowSize(window, &this->state->screen->width, &this->state->screen->height);
    // SDL_GetWindowSize(window, &this->state->camera->width, &this->state->camera->height);
    this->state->camera->setSize(this->state->screen->width, this->state->screen->height);
}

void Window::setScene(Scene* scene) {
    std::cout << "Setting scene renderer to state\n";
    state->renderer = scene->renderer;
    std::cout << "Setting scene to window\n";
    this->scene = scene;
    std::cout << "Resizing window to scene size\n";
    SDL_GetWindowSize(window, &scene->width, &scene->height);
    SDL_GetWindowSize(window, &this->state->screen->width, &this->state->screen->height);
    // SDL_GetWindowSize(window, &this->state->camera->width, &this->state->camera->height);
    this->state->camera->setSize(this->state->screen->width, this->state->screen->height);
}

void Window::onResize(int width, int height) {
    scene->width = width;
    scene->height = height;
    // state->camera->setWidth(width);
    // state->camera->setHeight(height);
    state->camera->setSize(width, height);
    state->screen->setSize(width, height);
}

int Window::run() {
    scene->prepare();

    bool running = true;

    while (!state->event->close) {
        state->clock->tick();
        state->event->fetch();


        // SDL_Event event;
        // while (SDL_PollEvent(&event)) {
        //     switch (event.type) {
        //         case SDL_QUIT:
        //         running = false;
        //         break;
        //         // Handle other events such as keyboard, mouse, etc.
        //     }
        // }

        scene->update(state);
        scene->render(state);        
    }
    // while (true) {
    //     state->clock->tick();
    //     scene->update(state);
    //     scene->render(state);
    //     SDL_Delay(100);
    // }

    // while(!state->event->close){
    //     state->clock->tick();
    //     state->event->fetch();
    //     scene->update(state);
    //     scene->render(state);
    //     SDL_Delay(100);
    // }

    return 0;
}
