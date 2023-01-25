#ifndef GAME_SCENE
#define GAME_SCENE

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
        map<int, Object*> objects;

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
            for (auto const& [id, object] : objects)
            {
                object->update(state);
            }
        }

        virtual void render(State* state) {
            clear();
            renderObjects(state);
            display();
        }

        virtual void clear() {
            SDL_RenderClear(renderer);
        }

        virtual void renderObjects(State* state) {
            for (auto const& [id, object] : objects)
            {
                object->render(state);
            }
        }

        virtual void display(int delay=1) {
            SDL_RenderPresent(renderer);
            SDL_Delay(delay);
        }

        ~Scene() {
            SDL_DestroyRenderer(renderer);
        }

};

#endif
