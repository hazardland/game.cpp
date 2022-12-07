#ifndef GAME_SCENE_H
#define GAME_SCENE_H

using namespace std;
#include <map>
#include <iostream>
#include <vector>

#include <SDL2/SDL_image.h>

#include <game/sprite.h>
#include <game/object.h>
#include <game/clock.h>
#include <game/input.h>
#include <game/camera.h>

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

            SDL_RendererInfo info;
            for(auto i=0; i<driverCount; ++i)
            {
                if(SDL_GetRenderDriverInfo(i, &info) == 0)
                {
                    printf("Driver %d: %s\n", i, info.name);
                }
            }            

            // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (SDL_GetRendererInfo(renderer, &info)==0) {
                    printf("Chosen driver: %s\n", info.name);
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


        virtual void update(Clock *clock, Input* input, Camera* camera) {
            for (auto &object: objects) {
                object->update(clock->delta, input);
            }
        }

        virtual void render(Camera* camera) {

            SDL_RenderClear(renderer);

            for (auto &object: objects) {
                object->render(camera);
            }

            SDL_RenderPresent(renderer);

            SDL_Delay(1);

        }

        ~Scene() {
            SDL_DestroyRenderer(renderer);
        }

};

#endif
