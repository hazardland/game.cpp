#ifndef GAME_WORLD
#define GAME_WORLD

using namespace std;

#include <iostream>

#include <SDL2/SDL_image.h>


#include <game/noise.h>
#include <game/object.hpp>
#include <game/state.hpp>
#include <game/map.hpp>
#include <game/tile.hpp>
#include <game/drag.hpp>

class World: public Object {

    public:

    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Surface* surface;
    SDL_Rect frame;

    int x;
    int y;
    int width;
    int height;
    float scale;

    // int variations;
    // vector<vector<int>> colors;

    bool modified = false;

    Drag* drag;

    Map* map;
    
    vector<vector<int>> grid;
    vector<Tile*> tiles;


    /**
     * @brief Stores general data about terrain and helps to generate it
     * @param width tile count in horizontal 
     * @param height tile count in vertical
     * @param scale is just a zoom during draw
     * @param variations Terrain variation count. ex.: 3 (1:Water, 2:Grass, 3:Trees)
     * @param colors Terrain variation rbg colors ex {{0,255,255}, {76,30,0}, {15,255,0}}
    */
    Terrain(SDL_Renderer* renderer, 
            int width, 
            int height, 
            float scale) {

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

        prepare();

        // printf("minimap width %d, height %d\n", width, height);
        for (int x = 0; x < width; x++)
        {
            grid.push_back(vector<int>());
            for (int y = 0; y < height; y++)
            {
                grid[x].push_back(0);                    
            }
        }

        drag = new Drag(&position, false, true);

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

    void set(int x, int y, int terrain) {
        grid[x][y] = terrain;
        vector<int> color = tiles[terrain]->color;
        SDL_Rect rect;
        rect.x = x*scale;
        rect.y = y*scale;
        rect.h = scale<1?1:scale;
        rect.w = scale<1?1:scale;
        SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, color[0], color[1], color[2]));
        modified = true;
    }

    void setMap(Map* map) {
        this->map = map;
    }
    // void setTerrain(int x, int y, int terrain) {

    // }
    // int colors[][3];

    // void setColors(int colors[][3]) {
    //     this->colors = &colors;
    // }

    void prepare() {
        if (texture!=NULL) {
            printf("destroyed terrain texture\n");
            SDL_DestroyTexture(texture);
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        modified = false;
    }

    virtual void update(State* state) {
        Mouse* mouse = state->event->mouse;
        if (this->map!=NULL) {
            if (mouse->leftClick || mouse->leftDragActive) {
                if (mouse->inside(&position)) {
                    // printf("click %d %d pos %d %d %d %d\n", mouse->x, mouse->y, position.x, position.y, position.w, position.h);
                    state->camera->x = ((mouse->x-position.x)/this->scale) * (map->getWidth()/this->width) - state->camera->width/2; 
                    state->camera->y = ((mouse->y-position.y)/this->scale) * (map->getHeight()/this->height) - state->camera->height/2;
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
        
        SDL_RenderCopy(renderer, texture, &frame, &position);

        if (map!=NULL) {
            SDL_Rect scope;
            scope.x = position.x + state->camera->x / (map->getWidth()/(this->width*this->scale));
            scope.y = position.y + state->camera->y / (map->getHeight()/(this->height*this->scale));
            scope.w = state->camera->width / (map->getWidth()/(this->width*this->scale)); 
            scope.h = state->camera->height / (map->getHeight()/(this->height*this->scale)); 
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
            SDL_RenderFillRect(renderer, &scope);
            SDL_SetRenderDrawColor(renderer, 102, 5, 51, 255);
            SDL_RenderDrawRect(renderer, &scope);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        }
        
    }

    /**
     * Generates terrain with noise
     * @param seed Seed for noise
     * @param intensity Intensity for noise. ex.: 0.01 (Lower is smoother higher is intense)
     * @param ranges Terrain ranges per variation between ex.: {0.3, 0.5, 1} (0 - 0.3: Water, 0.3 - 0.5: Grass, 0.5 - 1: Trees)
    */
    void generate1(int seed, 
                   float intensity,
                   vector<float> ranges) {

        OpenSimplexNoise::Noise noise{seed};
        for (int x=0; x<width; x++) {
            for (int y=0; y<height; y++)
            {
                // alpha = (noise.eval(x*0.01, y*0.01) + 1) / 2.0  * 255.0;
                // minimap->setPixel(x, y, 255, 255, 255, alpha);       
                float depth = (noise.eval(x*intensity, y*intensity) + 1) / 2.0;
                for (int terrain=0; terrain<tiles.size(); terrain++){
                    if (depth<=ranges[terrain]) {
                        set(x, y, terrain);
                        break;
                    }
                }
            }
        }
        map->terrain = this->grid;
    }

    /**
     * @brief Generate terrain with guaranteed odd number 
     * of same tiles min in same place
     * @param seed Seed for noise
     * @param intensity Intensity for noise. ex.: 0.01 (Lower is smoother higher is intense)
     * @param ranges Terrain ranges per variation between ex.: {0.3, 0.5, 1} (0 - 0.3: Water, 0.3 - 0.5: Grass, 0.5 - 1: Trees)
    */
    void generate2(
                 int seed, 
                 float intensity,
                 vector<float> ranges) {

        OpenSimplexNoise::Noise noise{seed};
        for (int x=0; x<width; x+=2) {
            for (int y=0; y<height; y+=2)
            {
                // alpha = (noise.eval(x*0.01, y*0.01) + 1) / 2.0  * 255.0;
                // minimap->setPixel(x, y, 255, 255, 255, alpha);       
                float depth = (noise.eval(x*intensity, y*intensity) + 1) / 2.0;
                for (int terrain=0; terrain<tiles.size(); terrain++){
                    if (depth<=ranges[terrain]) {
                        set(x, y, terrain);
                        set(x+1, y, terrain);
                        set(x, y+1, terrain);
                        set(x+1, y+1, terrain);
                        break;
                    }
                }
            }
        }
        map->terrain = this->grid;
    }  

    /**
     * @brief Import chunk of map based on data
     * @param data 2 dimensionall data with different terrain variation types
    */
    void import(vector<vector<int>> data) {
        int height = data.size();
        int width = data[0].size();
        for (int y=0; y<height; y++)
        for (int x=0; x<width; x++)
        {
            set(x, y, data[y][x]);
        }
        map->terrain = this->grid;
    }
  

    /**
     * @brief Fill map by expanding edge strategy
    */
    void fillMapShrinked() {
        for (int x=0; x<width; x++) {
            for (int y=0; y<height; y++) {
                map->tiles[x][y] = getTileShrinked(x, y);
            }
        }
    }

    /**
     * @brief Choose tile for terrain when same type tile cluster contains min even number of tiles
     * It shrinks tile when detects other type of tile neighbours
    */
    int getTileShrinked(int x, int y) {
        // Get corner sum
        int type = this->grid[x][y];
        Tile* tile = tiles[type];
        if (type==0) {
            return tile->getPlain();
        }
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
            borders[0] = this->grid[x-1][y-1]<type ? 0 : 1;
        }
        if (y!=0){
            borders[1] = this->grid[x  ][y-1]<type ? 0 : 1;
        }
        if (y!=0 && x<this->width-1) {
            borders[2] = this->grid[x+1][y-1]<type ? 0 : 1;
        }
        if (x!=0) {
            borders[3] = this->grid[x-1][y  ]<type ? 0 : 1;
        }
        if (x<this->width-1) {
            borders[5] = this->grid[x+1][y  ]<type ? 0 : 1;
        }
        if (x!=0 && y<this->height-1) {
            borders[6] = this->grid[x-1][y+1]<type ? 0 : 1;
        }
        if (y<this->height-1){
            borders[7] = this->grid[x  ][y+1]<type ? 0 : 1;
        }
        if (x<this->width-1 && y<this->height-1) {
            borders[8] = this->grid[x+1][y+1]<type ? 0 : 1;
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
        
        // Mark bad tile
        if (result=="1001") {
            this->setPixel(x, y, 0, 0, 0, 255);
        }

        if (result.compare("1111")==0) {
            return tile->getPlain();
        }
        return tile->getEdge(result);
    }


};


#endif
