#ifndef GAME_MINIMAP
#define GAME_MINIMAP

using namespace std;

#include <iostream>

#include <SDL2/SDL_image.h>

#include <game/object.hpp>
#include <game/state.hpp>
#include <game/noise.h>

class Minimap: public Object {

    public:

    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Surface* surface;
    SDL_Rect frame;

    int x;
    int y;
    int width;
    int height;
    int scale;

    bool modified = false;

    Minimap(SDL_Renderer* renderer, int width, int height, int scale) {

        this->width = width;
        this->height = height;
        this->scale = scale;

        this->renderer = renderer;

        position.x = frame.x = 0;
        position.y = frame.y = 0;
        position.w = frame.w = width * scale;
        position.h = frame.h = height * scale;

        surface = SDL_CreateRGBSurfaceWithFormat(0, width*scale, height*scale, 0, SDL_PIXELFORMAT_RGBA32);
        // SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);

        updateSurface();
    }
    void setPixel(int x, int y, int red, int blue, int green) {
        SDL_Rect rect;
        rect.x = x*scale;
        rect.y = y*scale;
        rect.h = scale;
        rect.w = scale;
        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, red, blue, green));
        modified = true;
    }
    void setPixel(int x, int y, int color[3]) {
        SDL_Rect rect;
        rect.x = x*scale;
        rect.y = y*scale;
        rect.h = scale;
        rect.w = scale;
        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, color[0], color[1], color[2]));
        modified = true;
    }
    void setPixel(int x, int y, int red, int blue, int green, int alpha) {
        SDL_Rect rect;
        rect.x = x*scale;
        rect.y = y*scale;
        rect.h = scale;
        rect.w = scale;
        SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, red, blue, green, alpha));
        modified = true;
    }
    // void setPixel(int x, int y, int color[4]) {
    //     SDL_Rect rect;
    //     rect.x = x;
    //     rect.y = y;
    //     rect.h = scale;
    //     rect.w = scale;
    //     SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, color[0], color[1], color[2], color[3]));
    //     modified = true;
    // }

    void updateSurface() {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        modified = false;
    }

    virtual void render(State* state) {
        if (modified) {
            updateSurface();
        }
        SDL_RenderCopy(renderer, texture, &frame, &position);
    }

    /**
     * Generates terrain using Open Simplex Noise algorithm
     * @param seed Noise seed
     * @param intensity Noise intensity. ex.: 0.01 (Lower is smoother higher is intense)
     * @param variations Terrain variation count. ex.: 3 (1:Water, 2:Grass, 3:Trees)
     * @param ranges Terrain ranges per variation between ex.: {0.3, 0.5, 1} (0 - 0.3: Water, 0.3 - 0.5: Grass, 0.5 - 1: Trees)
     * @param colors Terrain variation rbg colors ex {{0,255,255}, {76,30,0}, {15,255,0}}
    */
    void generateTerrain(
                 int seed, 
                 float intensity,
                 int variations,
                 float ranges[],
                 int colors[][3]) {

        OpenSimplexNoise::Noise noise{seed};
        for (int y=0; y<width; y++) {
            for (int x=0; x<height; x++)
            {
                // alpha = (noise.eval(x*0.01, y*0.01) + 1) / 2.0  * 255.0;
                // minimap->setPixel(x, y, 255, 255, 255, alpha);       
                float height = (noise.eval(x*intensity, y*intensity) + 1) / 2.0;
                for (int i=0; i<variations; i++){
                    if (height<=ranges[i]) {
                        setPixel(x, y, colors[i]);
                        break;
                    }
                }
            }
        }
    }

};

#endif
