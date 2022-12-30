#ifndef TEST_SCENE_PERLIN
#define TEST_SCENE_PERLIN

using namespace std;
#include <vector>
#include <map>

#include <game/scene.hpp>
#include <game/text.hpp>
#include <game/map.hpp>
#include <game/legend.hpp>

int WIDTH = 128;
int HEIGHT = 128;

class MapScene : public Scene {
    using Scene::Scene;
    // World* world;
    Map* map;
    Legend* legend;
    TTF_Font* font = TTF_OpenFont("assets/fonts/titillium.ttf", 20);
    Text* fps;
    int ticks;

    public:

    virtual void prepare() {
        fps = new Text(renderer, font);
        
        // world = new World(WIDTH, HEIGHT);
        
        map = new Map(
            new Image(renderer, "assets/sprites/winter.png"), 
            32, 32, 
            WIDTH, HEIGHT, 
            3,
            new Text(renderer, font)
        );

        legend = new Legend(renderer ,WIDTH, HEIGHT, 2, map);
        map->setLegend(legend);
        map->debug = true;

        //world->setMap(map);
        // world->setLegend(legend);

        legend->setPosition(0, 0);

        objects.push_back(map);
        objects.push_back(legend);
        objects.push_back(fps);


/**

"1.1.1.x" = {0},
"1.1.x.x" = {1},
"1.1.x.1" = {2},
"1.x.1.x" = {28},
"x.1.x.1" = {30},
"1.x.1.1" = {56},
"x.x.1.1" = {57},
"x.1.1.1" = {58},
"x.x.x.1" = {3},
"x.x.1.x" = {4},
"x.1.x.x" = {31},
"1.x.x.x" = {32},
"x.1.1.x" = {59},
"1.x.x.1" = {60},
"1.1.1.1" = {5, 6, 33, 34}

"2.2.2.x" = {84},
"2.2.x.x" = {85},
"2.2.x.2" = {86},
"2.x.2.x" = {112},
"x.2.x.2" = {114},
"2.x.2.2" = {140},
"x.x.2.2" = {141},
"x.2.2.2" = {142},
"x.x.x.2" = {87},
"x.x.2.x" = {88},
"x.2.x.x" = {115},
"2.x.x.x" = {116},
"x.2.2.x" = {143},
"2.x.x.2" = {144},
"2.2.2.2" = {89, 117}

"0.0.0.1" = {168},
"0.0.1.1" = {169},
"0.0.1.0" = {170},
"0.1.0.1" = {196},
"1.0.1.0" = {198},
"0.1.0.0" = {224},
"1.1.0.0" = {225},
"1.0.0.0" = {226},
"1.1.1.0" = {171},
"1.1.0.1" = {172},
"1.0.1.1" = {199},
"0.1.1.1" = {200},
"1.0.0.1" = {227},
"0.1.1.0" = {228},
"0.0.0.0" = {173, 174, 201, 202, 229}


*/
        map->terrains = {
            new Terrain(0, {51, 51, 255}),
            new Terrain(1, {102, 178, 215}),
            new Terrain(2, {255, 255, 255})
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
            {"1.2.2.1", {278}} //271
        };


        generate();
    }

    void generate() {
        srand(clock());
        map->generate2(rand(), 0.05, {0.3, 0.55, 1});
        map->import(
            {
                {0,1,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {1,1,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0},
                {0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0},
                {0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0},
                {1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0},
                {1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0},
                {0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,0,0,0,0},
                {0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,0},
                {0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
            }
        );
        map->fillMap2();
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
        display();
    }

};

#endif
