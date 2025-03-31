// File: scene.cpp

#include <iostream>
#include <cmath>

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

    // SDL_RendererInfo driverInfo;
    const char* preferedDriverName = "direct3d11";
    // int preferedDriverId = -1;
    // for(int i=0; i<driverCount; ++i)
    // {
    //     if(SDL_GetRenderDriverInfo(i, &driverInfo) == 0)
    //     {
    //         if (strcmp(driverInfo.name, preferedDriverName)==0) {
    //             preferedDriverId = i;
    //             std::cout << "[Driver match " << preferedDriverId << "]" << std::endl;
    //         }
    //         std::cout << "Driver " << i << ": " << driverInfo.name << std::endl;
    //     }
    // }

    renderer = SDL_CreateRenderer(window->getInstance(), preferedDriverName);
    // renderer = SDL_CreateRenderer(window, preferedDriverId, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    std::cout << "Chosen driver: " << SDL_GetRendererName(renderer) << std::endl;

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

void Scene::prepare(State* state) {
    // Empty in base class, can be overridden by derived classes
}

void Scene::update(State* state) {

    if (client!=nullptr) {
        for (int i = 0; i < 10; ++i) {
            client->poll();
        }    
    }

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

    if (client!=nullptr) {
        client->flush();
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

    // int tileSize = 8;
    // int lightRadius = 256;               // in tiles
    // int maxDistance = lightRadius * tileSize;
    
    // int coreRadius = 96;               // 🔥 Light fully visible up to this many pixels
    
    // int lightX = width / 2;
    // int lightY = height / 2;
    
    // for (int x = 0; x < width; x += tileSize) {
    //     for (int y = 0; y < height; y += tileSize) {
    //         int centerX = x + tileSize / 2;
    //         int centerY = y + tileSize / 2;
    
    //         float dx = centerX - lightX;
    //         float dy = centerY - lightY;
    //         float distance = std::sqrt(dx * dx + dy * dy);
    
    //         float fadedDistance = std::max(distance - coreRadius, 0.0f);  // starts fading after core
    //         float alpha = std::clamp((fadedDistance / (maxDistance - coreRadius)) * 255.0f, 0.0f, 255.0f);
    
    //         SDL_FRect rect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(tileSize), static_cast<float>(tileSize) };
    //         SDL_SetRenderDrawColor(renderer, 0, 0, 0, static_cast<Uint8>(alpha));
    //         SDL_RenderFillRect(renderer, &rect);
    //     }
    // }
    
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

void Scene::addObject(Object* obj, uint32_t id) {
    objects.push_back(obj);
    objectById[obj->getId()] = obj;    
}

Object* Scene::getObject(int id) {
    auto it = objectById.find(id);
    return it != objectById.end() ? it->second : nullptr;
}

void Scene::removeObject(Object* obj) {
    // Remove from map
    int id = obj->getId();
    objectById.erase(id);

    // Remove from vector (stable erase, order preserved)
    auto it = std::find(objects.begin(), objects.end(), obj);
    if (it != objects.end()) {
        objects.erase(it);
    }
}

void Scene::removeObject(int id) {
    auto it = objectById.find(id);
    if (it != objectById.end()) {
        Object* obj = it->second;
        objectById.erase(it);

        // Remove from vector
        auto vit = std::find(objects.begin(), objects.end(), obj);
        if (vit != objects.end()) {
            objects.erase(vit);
        }
    }
}


Scene::~Scene() {
    SDL_DestroyRenderer(renderer);
}
