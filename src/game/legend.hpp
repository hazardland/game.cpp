#ifndef GAME_LEGEND
#define GAME_LEGEND

using namespace std;

#include <iostream>
#include <map>

#include <SDL2/SDL_image.h>


#include <game/object.hpp>
#include <game/state.hpp>
#include <game/drag.hpp>

class Legend: public Object {

    public:

    SDL_Renderer* renderer;
    SDL_Texture* texture = NULL;
    SDL_Surface* surface;
    SDL_Rect frame;

    Object* map;

    int x;
    int y;
    int width;
    int height;
    float scale;

    // int variations;
    // vector<vector<int>> colors;

    bool modified = false;

    Drag* drag;    

    /**
     * @brief Stores general data about terrain and helps to generate it
     * @param width tile count in horizontal 
     * @param height tile count in vertical
     * @param scale is just a zoom during draw
     * @param variations Terrain variation count. ex.: 3 (1:Water, 2:Grass, 3:Trees)
     * @param colors Terrain variation rbg colors ex {{0,255,255}, {76,30,0}, {15,255,0}}
    */
    Legend(SDL_Renderer* renderer, 
           int width, 
           int height, 
           float scale,
           Object* map
           ) {

        this->map = map;

        this->width = width;
        this->height = height;
        this->scale = scale;

        this->renderer = renderer;

        frame.x = 0;
        frame.y = 0;
        frame.w = width * scale;
        frame.h = height * scale;
        setPosition(0, 0);
        setSize(width * scale, height * scale);
        surface = SDL_CreateRGBSurfaceWithFormat(0, getWidth(), getHeight(), 0, SDL_PIXELFORMAT_RGBA32);

        prepare();

        drag = new Drag(getPosition(), false, true);

    }
    void setPixel(int x, int y, int red, int blue, int green) {
        SDL_Rect rect;
        rect.x = x*scale;
        rect.y = y*scale;
        rect.h = scale<1?1:scale;
        rect.w = scale<1?1:scale;
        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, red, blue, green));
        modified = true;
    }
    void setPixel(int x, int y, int red, int blue, int green, int alpha) {
        SDL_Rect rect;
        rect.x = x*scale;
        rect.y = y*scale;
        rect.h = scale<1?1:scale;
        rect.w = scale<1?1:scale;
        SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, red, blue, green, alpha));
        modified = true;
    }

    void prepare() {
        if (texture!=NULL) {
            printf("Destroyed legend texture\n");
            SDL_DestroyTexture(texture);
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        modified = false;
    }

    virtual void update(State* state) {
        Mouse* mouse = state->event->mouse;
        if (this->map!=NULL) {
            if (mouse->leftClick || mouse->leftDragActive) {
                if (mouse->inside(getPosition())) {
                    // printf("click %d %d pos %d %d %d %d\n", mouse->x, mouse->y, getX(), getY(), position.w, position.h);
                    state->camera->x = ((mouse->x-getX())/this->scale) * (map->getWidth()/this->width) - state->camera->width/2; 
                    state->camera->y = ((mouse->y-getY())/this->scale) * (map->getHeight()/this->height) - state->camera->height/2;
                    // printf("Setting camera %d %d\n", state->camera->x, state->camera->y);
                }
            }
        }
        drag->update(state);
    }

    virtual void render(State* state) {
        if (modified) {
            prepare();
        }
        
        SDL_RenderCopyF(renderer, texture, &frame, getPosition());

        if (map!=NULL) {
            SDL_Rect scope;
            scope.x = getX() + state->camera->x / (map->getWidth()/(this->width*this->scale));
            scope.y = getY() + state->camera->y / (map->getHeight()/(this->height*this->scale));
            scope.w = state->camera->width / (map->getWidth()/(this->width*this->scale)); 
            scope.h = state->camera->height / (map->getHeight()/(this->height*this->scale)); 
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
            SDL_RenderFillRect(renderer, &scope);
            SDL_SetRenderDrawColor(renderer, 102, 5, 51, 255);
            SDL_RenderDrawRect(renderer, &scope);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }
        
    }


};


#endif
