#ifndef GAME_SCENE
#define GAME_SCENE

using namespace std;

#include <map>
#include <iostream>
#include <vector>
#include <list>

#include <SDL2/SDL_image.h>

#include <game/sprite.hpp>
#include <game/object.hpp>
#include <game/state.hpp>
#include <game/map.hpp>
#include <game/minimap.hpp>
// #include <game/clock.hpp>
// #include <game/input.hpp>
// #include <game/camera.hpp>

#include <thread>
#include <mutex>
#include <condition_variable>

class Scene {

    // list<Object*> visibleObjects;
   
    std::thread updateThread;
    std::mutex updateMutex;
    std::condition_variable updateCondition;
    bool shouldUpdate = false;
    bool stopUpdates = false;
    State* pendingStateUpdate = nullptr;

    list<Object*> visibleObjects[2];
    int currentVisibleObjects = 0; // index of the currently-visible objects
    std::mutex visibleObjectsMutex;

    public:

        int width;
        int height;

        SDL_Renderer *renderer;
        SDL_Window *window;

        map<int, Sprite*> sprites;
        // map<int, Object*> objects;
        vector<Object*> objects;

        Map* map;
        Minimap* minimap;

        Scene(SDL_Window* window, int width=0, int height=0) {

            this->window = window;
            this->width = width;
            this->height = height;

            int driverCount = SDL_GetNumRenderDrivers();
            printf("Number of renderer drivers: %d\n", driverCount);

            SDL_RendererInfo driverInfo;
            const char* preferedDriverName = "direct3d11";
            int preferedDriverId = -1;
            for(int i=0; i<driverCount; ++i)
            {
                if(SDL_GetRenderDriverInfo(i, &driverInfo) == 0)
                {
                    if (strcmp(driverInfo.name, preferedDriverName)==0) {
                        preferedDriverId = i;
                        printf("[Driver match %d]\n", preferedDriverId);
                    }
                    printf("Driver %d: %s\n", i, driverInfo.name);
                }
            }            

            //preferedDriverId = -1;

            // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            renderer = SDL_CreateRenderer(window, preferedDriverId, SDL_RENDERER_ACCELERATED);
            if (SDL_GetRendererInfo(renderer, &driverInfo)==0) {
                    printf("Chosen driver: %s\n", driverInfo.name);
            }


            if(!renderer){
                printf("Failed to create renderer: %s", SDL_GetError());
                SDL_DestroyWindow(window);
                SDL_Quit();
            }

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        }

        void setSize(int width, int height) {
            this->width = width;
            this->height = height;
        }

        virtual void prepare() {

        };


        // virtual void update(State* state) {
        //     if (map) {
        //         map->update(state);
        //     }
        //     visibleObjects.clear();
        //     for (auto const& [id, object] : objects)
        //     {
        //         object->update(state);
        //         if (object->isVisible(state)) {
        //             visibleObjects.push_back(object);
        //         }
        //     }
        //     if (minimap) {
        //         minimap->update(state);
        //     }
        // }
        // virtual void update(State* state) {
        //     if (map) {
        //         map->update(state);
        //     }
        //     visibleObjects.clear();
        //     for (Object* object : objects)  // We change the iteration to be simpler
        //     {
        //         object->update(state);
        //         if (object->isVisible(state)) {
        //             visibleObjects.push_back(object);
        //         }
        //     }
        //     if (minimap) {
        //         minimap->update(state);
        //     }
        // }

    virtual void update(State* state) {
        std::lock_guard<std::mutex> lock(updateMutex);
        pendingStateUpdate = state;
        shouldUpdate = true;
        updateCondition.notify_one();
    }

   void startUpdating() {
        updateThread = std::thread([this]() {
            while (true) {
                State* state;
                {
                    std::unique_lock<std::mutex> lock(updateMutex);
                    updateCondition.wait(lock, [this] { return shouldUpdate || stopUpdates; });
                    if (stopUpdates) break;
                    state = pendingStateUpdate;
                    shouldUpdate = false;
                }
                // Perform update...
                if (map) {
                    map->update(state);
                }
                int nextVisibleObjects = 1 - currentVisibleObjects; // index of the next set of visible objects
                visibleObjects[nextVisibleObjects].clear();
                for (Object* object : objects)
                {
                    object->update(state);
                    if (object->isVisible(state)) {
                        visibleObjects[nextVisibleObjects].push_back(object);
                    }
                }
                if (minimap) {
                    minimap->update(state);
                }
                {
                    std::lock_guard<std::mutex> lock(visibleObjectsMutex);
                    currentVisibleObjects = nextVisibleObjects;
                }
            }
        });
    }

        virtual void render(State* state) {
            clear();
            build(state);
            present();
        }

        virtual void clear() {
            SDL_RenderClear(renderer);
        }

    virtual void build(State* state) {
        map->render(state);
        std::lock_guard<std::mutex> lock(visibleObjectsMutex);
        for (Object* object : visibleObjects[currentVisibleObjects])
        {
            object->render(state);
        }
        minimap->render(state);
    }
        virtual void present(int delay=1) {
            SDL_RenderPresent(renderer);
            SDL_Delay(delay);
        }


        void addObject(Object* obj) {
            objects.push_back(obj);
        }

    ~Scene() {
        {
            std::lock_guard<std::mutex> lock(updateMutex);
            stopUpdates = true;
        }
        updateCondition.notify_one();
        if (updateThread.joinable()) {
            updateThread.join();
        }
        SDL_DestroyRenderer(renderer);
    }

};

#endif
