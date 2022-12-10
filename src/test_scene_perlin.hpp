#ifndef TEST_SCENE_PERLIN
#define TEST_SCENE_PERLIN

#include <game/scene.hpp>
#include <game/minimap.hpp>

// We will have 6 terrain types
int TERRAIN_COLORS[6][3] = {
    {51, 51, 255},
    {0, 0, 255},
    {0, 153, 0},
    {255, 153, 51},
    {96, 96, 96},
    {255, 255, 255}
};

// And we declare noise range per terrain type
float TERRAIN_RANGES[6] = {
    0.3,
    0.35,
    0.5,
    0.7,
    0.9,
    1
};


class TestScenePerlin : public Scene {
    using Scene::Scene;
    Minimap* minimap;
    int ticks;

    public:

    virtual void prepare() {
        minimap = new Minimap(renderer, 512, 512, 1);
        objects.push_back(minimap);
    }

    void update(State* state) {
        if (SDL_GetTicks()-ticks>1000) {
            srand(clock());
            minimap->generateTerrain(rand(), 0.01, 6, TERRAIN_RANGES, TERRAIN_COLORS);
            ticks = SDL_GetTicks();
        }
    }

};

#endif
