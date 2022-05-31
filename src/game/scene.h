#ifndef GAME_SCENE_H
#define GAME_SCENE_H

using namespace std;
#include <map>
#include <iostream>
#include <vector>

#include <SDL2/SDL_image.h>

#include <game/sprite.h>
#include <game/unit.h>
#include <game/clock.h>
#include <game/input.h>

#include <enum.h>
#include <footman.h>

class Scene {

    public:

        int width;
        int height;

        SDL_Renderer *renderer;
        SDL_Window *window;

        map<int, Sprite*> sprites;
        vector<Unit*> units;


        Scene(SDL_Window* window, int width, int height) {

            this->window = window;
            this->width = width;
            this->height = height;

            // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            if(!renderer){
                printf("Failed to create renderer: %s", SDL_GetError());
                SDL_DestroyWindow(window);
                SDL_Quit();
            }

        }

        virtual void prepare() {

        };


        virtual void update(Clock *clock, Input* input) {
            for (auto &unit: units) {
                unit->update(clock->delta, input);
            }
        }

        virtual void render() {

            SDL_RenderClear(renderer);

            for (auto &unit: units) {
                unit->render(renderer);
            }

            SDL_RenderPresent(renderer);

            SDL_Delay(1);

        }

        ~Scene() {
            SDL_DestroyRenderer(renderer);
        }

};

#endif
