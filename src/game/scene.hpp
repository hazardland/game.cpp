#ifndef GAME_SCENE_H
#define GAME_SCENE_H

using namespace std;

#include <map>
#include <iostream>
#include <vector>

#include <SDL2/SDL_image.h>

#include <game/sprite.hpp>
#include <game/object.hpp>
#include <game/state.hpp>
// #include <game/clock.hpp>
// #include <game/input.hpp>
// #include <game/camera.hpp>

class Scene {

    public:

        int width;
        int height;

        SDL_Renderer *renderer;
        SDL_Window *window;

        map<int, Sprite*> sprites;
        vector<Object*> objects;


        Scene(SDL_Window* window, int width=0, int height=0) {

            this->window = window;
            this->width = width;
            this->height = height;

            int driverCount = SDL_GetNumRenderDrivers();
            printf("Number of renderer drivers: %d\n", driverCount);

            SDL_RendererInfo driverInfo;
            const char* preferedDriverName = "opengl";
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

            preferedDriverId = -1;

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


        virtual void update(State* state) {
            for (auto &object: objects) {
                object->update(state);
            }
        }

        virtual void render(State* state) {

            SDL_RenderClear(renderer);

            for (auto &object: objects) {
                object->render(state);
            }

            SDL_RenderPresent(renderer);

            SDL_Delay(1);

        }

        ~Scene() {
            SDL_DestroyRenderer(renderer);
        }

};

#endif
