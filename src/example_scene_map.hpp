#ifndef TEST_SCENE_PERLIN
#define TEST_SCENE_PERLIN

using namespace std;
#include <vector>
#include <map>

#include <game/scene.hpp>
#include <game/terrain.hpp>
#include <game/map.hpp>
#include <game/text.hpp>

int WIDTH = 512;
int HEIGHT = 512;

class Tile {
    public:
        vector<int> plain;
        map<string, vector<int>> edges;
    Tile (vector<int> plain, map<string, vector<int>> edges={}) {
        this->plain = plain;
        this->edges = edges;
    }
    int random(int min, int max) {
        return rand() % (max-min+1) + min;   
    }
    int getPlain() {
        return plain[random(0, plain.size()-1)];
    }
    int getEdge(string key) {
        if (key=="1001") cout << key << "\n";
        return edges[key][random(0, edges[key].size()-1)];
    }
    int getTile(Terrain* terrain, int x, int y) {
        // Get corner sum
        int type = terrain->grid[x][y];
        //   x x x
        // y 0 0 0
        // y 0 0 0
        // y 0 0 0
        int borders[] = {
            1, 1, 1, // 0 1 2 
            1, 1, 1, // 3 4 5
            1, 1, 1  // 6 7 8
        };
        int corners[] = {
            0, 0, // 0 1
            0, 0  // 2 3
        };
        if (x!=0 && y!=0){
            borders[0] = terrain->grid[x-1][y-1]<type ? 0 : 1;
        }
        if (y!=0){
            borders[1] = terrain->grid[x  ][y-1]<type ? 0 : 1;
        }
        if (y!=0 && x<terrain->width-1) {
            borders[2] = terrain->grid[x+1][y-1]<type ? 0 : 1;
        }
        if (x!=0) {
            borders[3] = terrain->grid[x-1][y  ]<type ? 0 : 1;
        }
        if (x<terrain->width-1) {
            borders[5] = terrain->grid[x+1][y  ]<type ? 0 : 1;
        }
        if (x!=0 && y<terrain->height-1) {
            borders[6] = terrain->grid[x-1][y+1]<type ? 0 : 1;
        }
        if (y<terrain->height-1){
            borders[7] = terrain->grid[x  ][y+1]<type ? 0 : 1;
        }
        if (x<terrain->width-1 && y<terrain->height-1) {
            borders[8] = terrain->grid[x+1][y+1]<type ? 0 : 1;
        }
        corners[0] = borders[3] + borders[0] + borders[1];  
        corners[1] = borders[1] + borders[2] + borders[5];  
        corners[2] = borders[3] + borders[6] + borders[7];  
        corners[3] = borders[7] + borders[8] + borders[5];

        int corner = 0;
        int min = corners[0];

        for(int i=1; i<4; i++)
        {
            if(corners[i]<min)
            {
                min = corners[i];
                corner = i;
            }
        }

        // int borders[] = {
        //     1, 1, 1, // 0 1 2 
        //     1, 1, 1, // 3 4 5
        //     1, 1, 1  // 6 7 8
        // };
        // int corners[] = {
        //     0, 0, // 0 1
        //     0, 0  // 2 3
        // };
        
        string result;
        if (corner==0) {
            result = 
            to_string(borders[0]) + 
            to_string(borders[1]) + 
            to_string(borders[3]) + 
            to_string(borders[4]);
        }
        if (corner==1) {
            result = 
            to_string(borders[1]) + 
            to_string(borders[2]) + 
            to_string(borders[4]) + 
            to_string(borders[5]);

        }
        if (corner==2) { 
            result = 
            to_string(borders[3]) + 
            to_string(borders[4]) + 
            to_string(borders[6]) + 
            to_string(borders[7]);

        }
        if (corner==3) {
            result = 
            to_string(borders[4]) + 
            to_string(borders[5]) + 
            to_string(borders[7]) + 
            to_string(borders[8]);            
        }

        // printf("-- x: %d y: %d c: %d\n%d %d %d\n%d %d %d\n%d %d %d\n",
        // x, y, corner, borders[0],borders[1], borders[2], borders[3], borders[4], borders[5], borders[6], borders[7], borders[8]        
        // );
        
        if (result=="1001") {
            terrain->setPixel(x, y, 0, 0, 0, 255);
        }

        if (result.compare("1111")==0) {
            return getPlain();
        }
        return getEdge(result);
    }
};

int TERRAIN_VARIATIONS = 3;

int TERRAIN_COLORS[3][3] = {
    {51, 51, 255},
    {102, 178, 215},
    {255, 255, 255}
};

float TERRAIN_RANGES[3] = {
    0.3,
    0.55,
    1
};

int TERRAIN_SCALE = 2;
int MAP_SCALE = 32;

class TestScenePerlin : public Scene {
    using Scene::Scene;
    Terrain* terrain;
    Map* map;
    Image* tileset;
    TTF_Font* font = TTF_OpenFont("assets/fonts/titillium.ttf", 32);
    vector<Tile*> tiles;
    int ticks;

    public:

    virtual void prepare() {
        tiles = {
            new Tile(
                {
                    319, 320, 321,
                    319, 320, 321,
                    319, 320, 321,
                    319, 320, 321,
                    322, 323, 324
                }
            ),
            new Tile(
                {
                    331, 332, 333, 334,
                    331, 332, 333, 334,
                    331, 332, 333, 334,
                    331, 332, 333, 334,
                    335, 336, 337, 338, 339
                },
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
            new Tile(
                {
                    349, 350, 351,
                    349, 350, 351,
                    349, 350, 351,
                    349, 350, 351,
                    352, 353, 354, 355, 356, 357, 358, 359, 363
                },
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
        tileset = new Image(renderer, "assets/sprites/winter.png");
        terrain = new Terrain(renderer, WIDTH, HEIGHT, 1);
        map = new Map(tileset, 32, 32, WIDTH, HEIGHT, 1);
        terrain->setMap(map);
        terrain->setPosition(0, 0);

        objects.push_back(map);
        objects.push_back(terrain);

        generate();
    }

    virtual void generate() {
        srand(clock());
        terrain->generate(rand(), 0.05, TERRAIN_VARIATIONS, TERRAIN_RANGES, TERRAIN_COLORS);
        for (int x=0; x<WIDTH; x++) {
            for (int y=0; y<HEIGHT; y++) {
                if (terrain->grid[x][y]==0) {
                    map->grid[x][y] = tiles[0]->getPlain();
                } else if (terrain->grid[x][y]==1) {
                    // map->grid[x][y] = 331;
                    map->grid[x][y] = tiles[1]->getTile(terrain, x, y);
                } else if (terrain->grid[x][y]==2) {
                    map->grid[x][y] = tiles[2]->getTile(terrain, x, y);;
                }
            }
        }
    }

    virtual void update(State* state) {
        // if (SDL_GetTicks()-ticks>1000) {
        //     tile++;
        //     ticks = SDL_GetTicks();
        // }

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

    virtual void render(State* state, bool present=true){
        Scene::render(state, false);
        this->present();
    }




};

#endif
