// File: scene.cpp

#include <iostream>

#include "game/scene.h"

#include "game/object.h"
#include "game/state.h"
#include "game/map.h"
#include "game/minimap.h"
#include "game/window.h"

Scene::Scene(Window* window, int width, int height) {

    this->window = window;
    int driverCount = SDL_GetNumRenderDrivers();
    std::cout << "Number of renderer drivers: " << driverCount << std::endl;

    SDL_RendererInfo driverInfo;
    const char* preferedDriverName = "direct3d11";
    int preferedDriverId = -1;
    for(int i=0; i<driverCount; ++i)
    {
        if(SDL_GetRenderDriverInfo(i, &driverInfo) == 0)
        {
            if (strcmp(driverInfo.name, preferedDriverName)==0) {
                preferedDriverId = i;
                std::cout << "[Driver match " << preferedDriverId << "]" << std::endl;
            }
            std::cout << "Driver " << i << ": " << driverInfo.name << std::endl;
        }
    }

    renderer = SDL_CreateRenderer(window->getInstance(), preferedDriverId, SDL_RENDERER_ACCELERATED);
    // renderer = SDL_CreateRenderer(window, preferedDriverId, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (SDL_GetRendererInfo(renderer, &driverInfo)==0) {
        std::cout << "Chosen driver: " << driverInfo.name << std::endl;
    }

    if(!renderer){
        std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window->getInstance());
        SDL_Quit();
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void Scene::setSize(int width, int height) {
    this->width = width;
    this->height = height;
}

void Scene::prepare() {
    // Empty in base class, can be overridden by derived classes
}

void Scene::update(State* state) {
    if (map!=nullptr) {
        map->update(state);
    }
    visibleObjects.clear();
    for (Object* object : objects)
    {
        object->update(state);
        if (object->isVisible(state)) {
            visibleObjects.push_back(object);
        }
    }
    if (minimap!=nullptr) {
        minimap->update(state);
    }
}

void Scene::render(State* state) {
    clear();
    build(state);
    present();
}

void Scene::clear() {
    SDL_RenderClear(renderer);
}

void Scene::build(State* state) {
    if (map!=nullptr) {
       map->render(state);
    }
    for (Object* object : visibleObjects)
    {
        object->render(state);
    }
    if (minimap!=nullptr){
        minimap->render(state);
    }
}

void Scene::present(int delay) {
    SDL_RenderPresent(renderer);
    // cout << "Presenting new frame\n";
    SDL_Delay(1);
}

void Scene::addObject(Object* obj) {
    objects.push_back(obj);
}

Scene::~Scene() {
    SDL_DestroyRenderer(renderer);
}
