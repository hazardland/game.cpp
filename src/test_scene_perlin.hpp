#ifndef TEST_SCENE_PERLIN
#define TEST_SCENE_PERLIN

using namespace std;
#include <iostream>
#include <map>
#include <vector>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <game/scene.hpp>
#include <game/minimap.hpp>
#include <game/noise.h>


class TestScenePerlin : public Scene {

    using Scene::Scene;
    
    Minimap* minimap;

    int ticks;
    int seed = 0;

    public:

    virtual void prepare() {

        minimap = new Minimap(renderer, 512, 512, 1);
        objects.push_back(minimap);

    }

    // void update(State* state) {
    //     if (SDL_GetTicks()-ticks>500) {
    //         int alpha;
    //         srand(clock());
    //         //float seed = rand();
    //         seed++;
    //         float step = 0.333;
    //         for (int y=0; y<100; y++) {
    //             for (int x=0; x<100; x++)
    //             {
    //                 // alpha = (noise((float)x*step+seed, (float)y*step+seed, 1.1) + 1) / 2.0  * 255.0;
    //                 alpha = (noise((float)x*step+seed, y, 1.1) + 1) / 2.0  * 255.0;
    //                 minimap->setPixel(x, y, 255, 255, 255, alpha);
    //             }

    //         }
    //         ticks = SDL_GetTicks();
    //     }
    // }

    void update(State* state) {
        if (SDL_GetTicks()-ticks>100) {
            int alpha;
            srand(clock());
            OpenSimplexNoise::Noise noise{rand()}; // Init
            for (int y=0; y<512; y++) {
                for (int x=0; x<512; x++)
                {
                    alpha = (noise.eval(x*0.01, y*0.01) + 1) / 2.0  * 255.0; // Use
                    minimap->setPixel(x, y, 255, 255, 255, alpha);
                }

            }
            ticks = SDL_GetTicks();
        }
    }

};

#endif
