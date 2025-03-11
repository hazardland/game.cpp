#ifndef TEST_SCENE_PERLIN
#define TEST_SCENE_PERLIN

#include <vector>
#include <map>

#include <game/scene.h>
#include <game/text.h>
#include <game/fps.h>
#include <game/map.h>
#include <game/minimap.h>
#include <game/terrain.h>
#include <game/image.h>
#include <game/select.h>


#include <examples/enum.h>
#include <examples/footman.hpp>
#include <examples/farm.hpp>

int WIDTH = 500;
int HEIGHT = 500;
int SPRITE_FOOTMAN_RED = 1;
int SPRITE_HUMAN_FARM = 2;

int LAYER_WATER = 0;
int LAYER_GROUND = 1;
int LAYER_AIR = 2;

int TERRAIN_WATER = 0;
int TERRAIN_SHORE = 1;
int TERRAIN_GROUND = 2;
int TERRAIN_FOREST = 3;

class Warcraft : public Scene {
    using Scene::Scene;
    // Map* map;
    // Minimap* minimap;
    TTF_Font* font;
    TTF_Font* fontSmall;
    Text* fps;
    // Text* footmanDebugText;

    int ticks;

    public:

    virtual void prepare() {
        font = TTF_OpenFont("assets/fonts/titillium.ttf", 20);
        fontSmall = TTF_OpenFont("assets/fonts/titillium.ttf", 10);
        fps = (new Fps(font))->setPositionFixed(true);
                
        map = new Map(
            new Image(renderer, "assets/sprites/winter.png"), 
            32, 32, 
            WIDTH, HEIGHT, 3,
            fontSmall
        );

        minimap = new Minimap(
            renderer, 
            300, 300, 
            WIDTH, HEIGHT, 2
        );

        map->setMinimap(minimap);
        // map->debug = false;

        minimap->setPosition(0, 0);

        // objects.insert({map->getId(), map});

        map->terrains = {
            // Water
            new Terrain(TERRAIN_WATER, LAYER_WATER, {51, 51, 255}),
            // Ice
            new Terrain(TERRAIN_SHORE, LAYER_GROUND, {102, 178, 215}),
            // Ground
            new Terrain(TERRAIN_GROUND, LAYER_GROUND, {255, 255, 255}),
            // Forest
            new Terrain(TERRAIN_FOREST, LAYER_GROUND, {34, 139, 34}) // Dark green for forests            
        };

        map->tiles = {
            // Base water
            {{"0.0.0.0"}, {319, 320, 321, 319, 320, 321, 319, 320, 321, 319, 320, 321 , 322, 323, 324}},
            {{"1.1.1.1"}, {331, 332, 333, 334, 331, 332, 333, 334, 331, 332, 333, 334, 331, 332, 333, 334, 331, 332, 333, 334 , 335, 336, 337, 338, 339}},
            // Ice crossing water            
            {"0.1.1.1", {199, 200}},
            {"1.0.1.1", {201, 202}},
            {"0.0.1.1", {203, 204, 205}},
            {"1.1.0.1", {206, 207}},
            {"0.1.0.1", {208, 209, 210}},
            {"0.0.0.1", {212, 213}},
            {"1.1.1.0", {214, 215}},
            {"1.0.1.0", {217, 218, 219}},
            {"0.0.1.0", {220, 221}},
            {"1.1.0.0", {222, 223, 224}},
            {"0.1.0.0", {225, 226}},
            {"1.0.0.0", {227, 228}},
            {"1.0.0.1", {174}}, //271
            // Base ground
            {"2.2.2.2", {349, 350, 351, 349, 350, 351, 349, 350, 351, 349, 350, 351 , 352, 353, 354, 355, 356, 357, 358, 359, 363}},
            // Ground crossing ice
            {"1.2.2.2", {259, 260}},
            {"2.1.2.2", {261, 262}},
            {"1.1.2.2", {263, 264, 265}},
            {"2.2.1.2", {266, 267}},
            {"1.2.1.2", {268, 269, 270}},
            {"1.1.1.2", {273, 274}},
            {"2.2.2.1", {275, 276}},
            {"2.1.2.1", {279, 280, 281}},
            {"1.1.2.1", {282, 283}},
            {"2.2.1.1", {284, 285, 286}},
            {"1.2.1.1", {287, 288}},
            {"2.1.1.1", {289, 290}},
            {"2.1.1.2", {174}}, //271
            {"1.2.2.1", {278}}, //271,
            // Base forest
            {"3.3.3.3", {108, 109, 111, 112, 113, 114, 119, 120}},
            // Forest crossing ground
            {"2.3.3.3", {129, 104}},
            {"3.2.3.3", {107, 131}},
            {"2.2.3.3", {106, 130}},
            {"3.3.2.3", {102, 127}},
            {"2.3.2.3", {115, 117, 128}},
            {"2.2.2.3", {129}},
            {"3.3.3.2", {133, 110}},
            {"3.2.3.2", {115, 118}},
            {"2.2.3.2", {131}},
            {"3.3.2.2", {124, 134}},
            {"2.3.2.2", {102}},
            {"3.2.2.2", {110}},
            {"3.2.2.3", {379}}, //372
            {"2.3.3.2", {379}}, //372,

        };

        std::cout << "Checkpoint" << std::endl;

        generate();


        sprites[SPRITE_FOOTMAN_RED] = (new Sprite(new Image(renderer, "assets/sprites/grunt.png"),
                                    72,
                                    72,
                                    100,
                                    true))
        ->addClip (MOVE+UP, 1, 2, 4, false, false)
        ->addClip (MOVE+DOWN, 5, 2, 4, false, false)
        ->addClip (MOVE+RIGHT, 3, 2, 4, false, false)
        ->addClip (MOVE+LEFT, 3, 2, 4, true, false)
        ->addClip (MOVE+UP+RIGHT, 2, 2, 4, false, false)
        ->addClip (MOVE+DOWN+RIGHT, 4, 2, 4, false, false)
        ->addClip (MOVE+UP+LEFT, 2, 2, 4, true, false)
        ->addClip (MOVE+DOWN+LEFT, 4, 1, 4, true, false)
        ->addClip (IDLE+UP, 1, 1, 1, false, false)
        ->addClip (IDLE+DOWN, 5, 1, 1, false, false)
        ->addClip (IDLE+RIGHT, 3, 1, 1, false, false)
        ->addClip (IDLE+LEFT, 3, 1, 1, true, false)
        ->addClip (IDLE+UP+RIGHT, 2, 1, 1, false, false)
        ->addClip (IDLE+DOWN+RIGHT, 4, 1, 1, false, false)
        ->addClip (IDLE+UP+LEFT, 2, 1, 1, true, false)
        ->addClip (IDLE+DOWN+LEFT, 4, 1, 1, true, false)
        ->addClip (ATTACK+UP, 1, 6, 4, false, false)
        ->addClip (ATTACK+DOWN, 5, 6, 4, false, false)
        ->addClip (ATTACK+RIGHT, 3, 6, 4, false, false)
        ->addClip (ATTACK+LEFT, 3, 6, 4, true, false)
        ->addClip (ATTACK+UP+RIGHT, 2, 6, 4, false, false)
        ->addClip (ATTACK+DOWN+RIGHT, 4, 6, 4, false, false)
        ->addClip (ATTACK+UP+LEFT, 2, 6, 4, true, false)
        ->addClip (ATTACK+DOWN+LEFT, 4, 6, 4, true, false);

        sprites[SPRITE_HUMAN_FARM] = (new Sprite(new Image(renderer, "assets/sprites/farm.png"),
                                    64,
                                    64,
                                    100,
                                    true))->addClip();


        for (int x=128; x<100*128; x+=128) {
            for (int y=128; y<100*128; y+=128) {
                Farm* farm = new Farm(sprites[SPRITE_HUMAN_FARM]);
                farm->setMap(map);
                if (farm->canExist(x-20, y-20, 64+20, 64+20)) {
                    farm->setPosition(x, y); 
                    addObject(farm);
                }

            }
        }

        // footmanDebugText = (new Text(fontSmall))
        //                     ->enableCache()
        //                     ->setPositionFixed(false);

        Footman* lastFootman = NULL;
        for (int x=0; x<100*32; x+=32) {
            for (int y=10; y<100*32; y+=32) {
                Footman* footman = new Footman(sprites[SPRITE_FOOTMAN_RED], fontSmall);
                footman->setMap(map);
                if (footman->canExist(x, y, 32, 32)) {
                    footman->setPosition(x, y);
                    addObject(footman);
                    lastFootman = footman;
                }
            }
        }
        if (lastFootman!=nullptr) {
            lastFootman->select();
        }

        // objects.insert({minimap->getId(), minimap});
        // objects.insert({fps->getId(), fps});
        addObject(fps);

        addObject(new Select({255,255,255,100},{1, 1, 1, 100}));
    }

    void generate() {
        srand(clock());
        map->generate(rand(), 0.05, {0.2, 0.45, 0.8, 1});
        // map->import(
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
    }

    // virtual void update(State* state) {

    //     // Keyboard* keyboard = state->event->keyboard; 

    //     // if (keyboard->down) {
    //     //     state->camera->y += 50;
    //     // }
    //     // if (keyboard->up) {
    //     //     state->camera->y -= 50;
    //     // }
    //     // if (keyboard->right) {
    //     //     state->camera->x += 50;
    //     // }
    //     // if (keyboard->left) {
    //     //     state->camera->x -= 50;
    //     // }

    //     // if (keyboard->space) {
    //     //     generate();
    //     // }

    //     Scene::update(state);

    // }

    // virtual void render(State* state){
    //     clear();
    //     build(state);
    //     display();
    // }

};

#endif

// /**

// "1.1.1.x" = {0},
// "1.1.x.x" = {1},
// "1.1.x.1" = {2},
// "1.x.1.x" = {28},
// "x.1.x.1" = {30},
// "1.x.1.1" = {56},
// "x.x.1.1" = {57},
// "x.1.1.1" = {58},
// "x.x.x.1" = {3},
// "x.x.1.x" = {4},
// "x.1.x.x" = {31},
// "1.x.x.x" = {32},
// "x.1.1.x" = {59},
// "1.x.x.1" = {60},
// "1.1.1.1" = {5, 6, 33, 34}

// "2.2.2.x" = {84},
// "2.2.x.x" = {85},
// "2.2.x.2" = {86},
// "2.x.2.x" = {112},
// "x.2.x.2" = {114},
// "2.x.2.2" = {140},
// "x.x.2.2" = {141},
// "x.2.2.2" = {142},
// "x.x.x.2" = {87},
// "x.x.2.x" = {88},
// "x.2.x.x" = {115},
// "2.x.x.x" = {116},
// "x.2.2.x" = {143},
// "2.x.x.2" = {144},
// "2.2.2.2" = {89, 117}

// "0.0.0.1" = {168},
// "0.0.1.1" = {169},
// "0.0.1.0" = {170},
// "0.1.0.1" = {196},
// "1.0.1.0" = {198},
// "0.1.0.0" = {224},
// "1.1.0.0" = {225},
// "1.0.0.0" = {226},
// "1.1.1.0" = {171},
// "1.1.0.1" = {172},
// "1.0.1.1" = {199},
// "0.1.1.1" = {200},
// "1.0.0.1" = {227},
// "0.1.1.0" = {228},
// "0.0.0.0" = {173, 174, 201, 202, 229}


// */
