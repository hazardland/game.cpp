#ifndef TEST_SCENE_PERLIN
#define TEST_SCENE_PERLIN

using namespace std;
#include <vector>
#include <map>

#include <game/scene.hpp>
#include <game/map.hpp>
#include <game/terrain.hpp>
#include <game/text.hpp>
#include <game/tile.hpp>

int WIDTH = 1024;
int HEIGHT = 1024;

class MapScene : public Scene {
    using Scene::Scene;
    Terrain* terrain;
    Map* map;
    TTF_Font* font = TTF_OpenFont("assets/fonts/titillium.ttf", 20);
    vector<Tile*> tiles1;
    vector<Tile*> tiles2;
    Text* fps;
    int ticks;

    public:

    virtual void prepare() {
        fps = new Text(renderer, font);
        
        terrain = new Terrain(renderer, WIDTH, HEIGHT, 0.2, 
            3, // Variations
            {  // Colors
                {51, 51, 255},
                {102, 178, 215},
                {255, 255, 255}
            }
        );
        
        map = new Map(
            new Image(renderer, "assets/sprites/winter.png"), 
            32, 32, 
            WIDTH, HEIGHT, 
            2,
            new Text(renderer, font)
        );

        tiles1 = {
            // Water
            new Tile(
                // Base water
                {
                    319, 320, 321,
                    319, 320, 321,
                    319, 320, 321,
                    319, 320, 321,
                    322, 323, 324
                }
            ),
            // Ice
            new Tile(
                // Base ice 
                {
                    331, 332, 333, 334,
                    331, 332, 333, 334,
                    331, 332, 333, 334,
                    331, 332, 333, 334,
                    335, 336, 337, 338, 339
                },
                // Ice crossing water
                {
                    {"0111", {199, 200}},
                    {"1011", {201, 202}},
                    {"0011", {203, 204, 205}},
                    {"1101", {206, 207}},
                    {"0101", {208, 209, 210}},
                    {"0001", {212, 213}},
                    {"1110", {214, 215}},
                    {"1010", {217, 218, 219}},
                    {"0010", {220, 221}},
                    {"1100", {222, 223, 224}},
                    {"0100", {225, 226}},
                    {"1000", {227, 228}},
                    {"1001", {174}} //271
                }
            ),
            // Ground
            new Tile(
                // Base ground
                {
                    349, 350, 351,
                    349, 350, 351,
                    349, 350, 351,
                    349, 350, 351,
                    352, 353, 354, 355, 356, 357, 358, 359, 363
                },
                // Ground crossing ice
                {
                    {"0111", {259, 260}},
                    {"1011", {261, 262}},
                    {"0011", {263, 264, 265}},
                    {"1101", {266, 267}},
                    {"0101", {268, 269, 270}},
                    {"0001", {273, 274}},
                    {"1110", {275, 276}},
                    {"1010", {279, 280, 281}},
                    {"0010", {282, 283}},
                    {"1100", {284, 285, 286}},
                    {"0100", {287, 288}},
                    {"1000", {289, 290}},
                    {"1001", {174}} //271
                }
            )
        };

        tiles2 = {
            // Water
            new Tile(
                // Base water
                {
                    319, 320, 321,
                    319, 320, 321,
                    319, 320, 321,
                    319, 320, 321,
                    322, 323, 324
                },
                // Ice crossing water
                {
                    {"1000", {199, 200}},
                    {"0100", {201, 202}},
                    {"1100", {203, 204, 205}},
                    {"0010", {206, 207}},
                    {"1010", {208, 209, 210}},
                    {"1110", {212, 213}},
                    {"0001", {214, 215}},
                    {"0101", {217, 218, 219}},
                    {"1101", {220, 221}},
                    {"0011", {222, 223, 224}},
                    {"1011", {225, 226}},
                    {"0111", {227, 228}},
                    {"0110", {174}} //271
                }                
            ),
            // Ice
            new Tile(
                // Base ice 
                {
                    331, 332, 333, 334,
                    331, 332, 333, 334,
                    331, 332, 333, 334,
                    331, 332, 333, 334,
                    335, 336, 337, 338, 339
                },
                // Ground crossing ice
                {
                    {"1000", {259, 260}},
                    {"0100", {261, 262}},
                    {"1100", {263, 264, 265}},
                    {"0010", {266, 267}},
                    {"1010", {268, 269, 270}},
                    {"1110", {273, 274}},
                    {"0001", {275, 276}},
                    {"0101", {279, 280, 281}},
                    {"1101", {282, 283}},
                    {"0011", {284, 285, 286}},
                    {"1011", {287, 288}},
                    {"0111", {289, 290}},
                    {"0110", {174}} //271
                }
            ),
            // Ground
            new Tile(
                // Base ground
                {
                    349, 350, 351,
                    349, 350, 351,
                    349, 350, 351,
                    349, 350, 351,
                    352, 353, 354, 355, 356, 357, 358, 359, 363
                }
            )
        };

        terrain->setMap(map);
        terrain->setPosition(0, 0);

        objects.push_back(map);
        objects.push_back(terrain);
        objects.push_back(fps);

        generate();
    }

    void generate() {
        srand(clock());
        terrain->generate1(rand(), 0.05, {0.3, 0.55, 1});
        // terrain->import(
        //     {
        //         {0,1,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
        //         {1,1,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
        //         {0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
        //         {1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0},
        //         {0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0},
        //         {0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0},
        //         {1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0},
        //         {1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0},
        //         {0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0},
        //         {0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        //         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        //         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        //         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
        //         {0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,0,0,0,0},
        //         {0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,0},
        //         {0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0},
        //         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        //         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        //         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        //         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        //     }
        // );
        fill2();
    }

    void fill1() {
        for (int x=0; x<WIDTH; x++) {
            for (int y=0; y<HEIGHT; y++) {
                if (terrain->grid[x][y]==0) {
                    map->grid[x][y] = tiles1[0]->getPlain();
                } else if (terrain->grid[x][y]==1) {
                    // map->grid[x][y] = 331;
                    map->grid[x][y] = tiles1[1]->getTile(terrain, x, y, false);
                } else if (terrain->grid[x][y]==2) {
                    map->grid[x][y] = tiles1[2]->getTile(terrain, x, y, false);
                }
            }
        }
    }

    void fill2() {
        for (int x=0; x<WIDTH; x++) {
            for (int y=0; y<HEIGHT; y++) {
                if (terrain->grid[x][y]==0) {
                    map->grid[x][y] = tiles2[0]->getTile(terrain, x, y, true);
                } else if (terrain->grid[x][y]==1) {
                    // map->grid[x][y] = 331;
                    map->grid[x][y] = tiles2[1]->getTile(terrain, x, y, true);
                } else if (terrain->grid[x][y]==2) {
                    map->grid[x][y] = tiles2[2]->getPlain();
                }
            }
        }
    }

    virtual void update(State* state) {
        if (SDL_GetTicks()-ticks>500) {
            fps->setText(to_string(state->clock->fps) + " FPS");
            fps->position.x = width-fps->getWidth()-5;
            ticks = SDL_GetTicks();
        }

        Keyboard* keyboard = state->event->keyboard; 

        if (keyboard->down) {
            state->camera->y += 50;
        }
        if (keyboard->up) {
            state->camera->y -= 50;
        }
        if (keyboard->right) {
            state->camera->x += 50;
        }
        if (keyboard->left) {
            state->camera->x -= 50;
        }

        if (keyboard->space) {
            generate();
        }

        Scene::update(state);

    }

    virtual void render(State* state){
        clear();
        process(state);
        present();
    }




};

#endif
