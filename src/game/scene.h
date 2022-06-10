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

            // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

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


        virtual void update(Clock *clock, Input* input) {
            for (auto &object: objects) {
                object->update(clock->delta, input);
            }
        }

        virtual void render() {

            SDL_RenderClear(renderer);

            for (auto &object: objects) {
                if (object->getY()<height && object->getX()<width && 
                    object->getX()+object->getWidth()>0 && object->getY()+object->getHeight()>0) {
                    object->render();
                }
            }

            SDL_RenderPresent(renderer);

            SDL_Delay(1);

        }

        ~Scene() {
            SDL_DestroyRenderer(renderer);
        }

};

#endif
