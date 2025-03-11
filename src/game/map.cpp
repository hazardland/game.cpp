#include "game/map.h"

#include "game/object.h"
#include "game/image.h"
#include "game/clip.h"
#include "game/state.h"
#include "game/text.h"
#include "game/terrain.h"
#include "game/minimap.h"
#include "game/cell.h"
#include "game/noise.h"
#include "game/frame.h"
#include "game/camera.h"
#include "game/event.h"

Map::Map(Image* image, 
         int tileWidth, 
         int tileHeight, 
         int tilesPerWidth, 
         int tilesPerHeight, 
         int layerCount, 
         TTF_Font* font) {

    this->image = image;
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    this->tilesPerWidth = tilesPerWidth;
    this->tilesPerHeight = tilesPerHeight;
    clip = new Clip(image, 
                        tileWidth, 
                        tileHeight, 
                        1, 1, 
                        (image->getWidth()/tileWidth)*(image->getHeight()/tileHeight));
    
    for (int x = 0; x < tilesPerWidth; x++)
    {
        grid.push_back(std::vector<Cell*>());
        for (int y = 0; y < tilesPerHeight; y++) {
            grid[x].push_back(new Cell(layerCount));
        }
    }
    this->text = new Text(font);
    this->text->setColor(SDL_Color(255, 255, 255))->enableCache();

}


float Map::getWidth() {
    return tilesPerWidth*tileWidth;
}

float Map::getHeight() {
    return tilesPerHeight*tileHeight;
}

void Map::render(State* state) {

    Camera* camera = state->camera;

    // Choose renderable tiles
    // @todo Code can be optimized furzer
    int xTileFrom = (camera->x / tileWidth);
    int xTileTo = (camera->width / tileWidth) + xTileFrom + 2;
    int yTileFrom = (camera->y / tileHeight);
    int yTileTo = (camera->height / tileHeight) + yTileFrom + 2;
    if (xTileFrom<0) xTileFrom = 0;
    if (yTileFrom<0) yTileFrom = 0;
    if (xTileTo>tilesPerWidth) xTileTo = tilesPerWidth;
    if (yTileTo>tilesPerHeight) yTileTo = tilesPerHeight;
    //printf("map render region: %d,%d x %d,%d\n", xTileFrom, yTileFrom, xTileTo, yTileTo);

    SDL_FRect location;
    location.w = tileWidth;
    location.h = tileHeight;
    SDL_FRect* position;

    for (int x = xTileFrom; x < xTileTo; x++)
    {
        for (int y = yTileFrom; y < yTileTo; y++)
        {
            //grid[x][y]
            location.x = x*tileWidth;
            location.y = y*tileHeight;
            if (state->camera->isVisible(&location)) {
                position = state->camera->translate(&location);
                image->render(grid[x][y]->rect, position);
                
                // Debug
                if (debug){

                    // printf("%d ", clip->getFrame(grid[x][y])->rect.x);
                    SDL_SetRenderDrawColor(image->renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRectF(image->renderer, position);
                    SDL_SetRenderDrawColor(image->renderer, 0, 0, 0, 0);

                    std::array<int, 4> borders = getTileBorders(x, y);

                    text->setText(std::to_string(borders[0])); //+"tl"
                    text->setPosition(position->x+1, position->y+1);
                    text->render(state);

                    text->setText(std::to_string(borders[1])); //+"tr"
                    text->setPosition(position->x+position->w-text->getWidth()-1, position->y+1);
                    text->render(state);

                    text->setText(std::to_string(borders[2])); //+"bl"
                    text->setPosition(position->x+1, position->y+position->h-text->getHeight()-1);
                    text->render(state);

                    text->setText(std::to_string(borders[3])); //+"br"
                    text->setPosition(position->x+position->w-text->getWidth()-1, position->y+position->h-text->getHeight()-1);
                    text->render(state);
                }
            }
        }
    }
}

// Implement setTerrain method
void Map::setTerrain(int x, int y, int type) {
    Terrain* terrain = terrains[type];
    grid[x][y]->terrain = terrain;
    if (minimap!=NULL) {
        minimap->setTerrain(x, y, terrain->color[0], terrain->color[1], terrain->color[2]);
    }
}

void Map::setMinimap(Minimap* minimap) {
    this->minimap = minimap;
    minimap->setMapData(grid, tileWidth, tileHeight);
    // minimap->updateMapSize(getWidth(), getHeight(), tileScale);
}

int Map::getTile(std::string key) {
    // cout << "requested tile " << key << "\n";        
    return tiles[key][random(0, tiles[key].size()-1)];        
}

// void Map::import(std::vector<std::vector<int>> data) {

void Map::import(std::vector<std::vector<int>> data) {
    int height = data.size();
    int width = data[0].size();
    for (int y=0; y<height; y++)
    for (int x=0; x<width; x++)
    {
        setTerrain(x, y, data[y][x]);
    }
    //map->grid = this->grid;
    printf("imported custom map\n");
    fillMap();
}

void Map::generate(
                int seed, 
                float intensity,
                std::vector<float> ranges) {

    OpenSimplexNoise::Noise noise{seed};
    for (int x=0; x<tilesPerWidth; x+=2) {
        for (int y=0; y<tilesPerHeight; y+=2)
        {
            // alpha = (noise.eval(x*0.01, y*0.01) + 1) / 2.0  * 255.0;
            // minimap->setPixel(x, y, 255, 255, 255, alpha);       
            float depth = (noise.eval(x*intensity, y*intensity) + 1) / 2.0;
            for (int terrain=0; terrain<terrains.size(); terrain++){
                if (depth<=ranges[terrain]) {
                    setTerrain(x, y, terrain);
                    setTerrain(x+1, y, terrain);
                    setTerrain(x, y+1, terrain);
                    setTerrain(x+1, y+1, terrain);
                    break;
                }
            }
        }
    }
    // fillMap2();
    // map->grid = this->grid;
    printf("generated map 2\n");
    fillMap();

}  

void Map::fillMap() {
    
    for (int x=0; x<tilesPerWidth; x++) {
        for (int y=0; y<tilesPerHeight; y++) {
            grid[x][y]->tile = calculateTile(x, y);
            grid[x][y]->rect = &clip->getFrame(grid[x][y]->tile)->rect;
        }
    }
    printf("filled map 2\n");
}

int Map::calculateTile(int x, int y) {

    std::array<int, 4> borders = getTileBorders(x, y);

    std::string result = 
        std::to_string(borders[0]) + "." +
        std::to_string(borders[1]) + "." +
        std::to_string(borders[2]) + "." +
        std::to_string(borders[3]);
    
    if (result.compare("1.1.1.1")==0) {
        return getTile("1.1.1.1");
    }
    return getTile(result);
}

// In a cold winter I made this algorithm on a paper
// Everybody had fever at home
// This scans neigboars and genrates an array [*,*,*,*] 
// Where each element encoded terrain type for 4 corners of tile (from top left clockwise)
// This is for smooth terrain transitions
std::array<int, 4> Map::getTileBorders(int x, int y) {
    // Get corner sum
    Cell* cell = this->grid[x][y]; 
    int type = cell->terrain->id;
    // Terrain* terrain = cell->terrain;

    if (type == 0) {
        return {0, 0, 0, 0}; // Simple return for type 0
    }
    //   x x x
    // y 0 0 0
    // y 0 0 0
    // y 0 0 0
    int borders[] = {
        type, type, type, // 0 1 2 
        type, type, type, // 3 4 5
        type, type, type  // 6 7 8
    };
    int corners[] = {
        0, 0, // 0 1
        0, 0  // 2 3
    };

    if (x!=0 && y!=0){
        borders[0] = this->grid[x-1][y-1]->terrain->id < type ? type-1 : type;
    }
    if (y!=0){
        borders[1] = this->grid[x  ][y-1]->terrain->id < type ? type-1 : type;
    }
    if (y!=0 && x<tilesPerWidth-1) {
        borders[2] = this->grid[x+1][y-1]->terrain->id < type ? type-1 : type;
    }
    if (x!=0) {
        borders[3] = this->grid[x-1][y  ]->terrain->id < type ? type-1 : type;
    }
    if (x<tilesPerWidth-1) {
        borders[5] = this->grid[x+1][y  ]->terrain->id < type ? type-1 : type;
    }
    if (x!=0 && y<tilesPerHeight-1) {
        borders[6] = this->grid[x-1][y+1]->terrain->id < type ? type-1 : type;
    }
    if (y<tilesPerHeight-1){
        borders[7] = this->grid[x  ][y+1]->terrain->id < type ? type-1 : type;
    }
    if (x<tilesPerWidth-1 && y<tilesPerHeight-1) {
        borders[8] = this->grid[x+1][y+1]->terrain->id < type ? type-1 : type;
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

    // Return the selected four border values based on the determined corner
    switch (corner) {
        case 0:
            return {borders[0], borders[1], borders[3], borders[4]};
        case 1:
            return {borders[1], borders[2], borders[4], borders[5]};
        case 2:
            return {borders[3], borders[4], borders[6], borders[7]};
        case 3:
            return {borders[4], borders[5], borders[7], borders[8]};
    }

    return {0, 0, 0, 0}; // Fallback (should never be reached)    
}

int Map::random(int min, int max) {
    return rand() % (max-min+1) + min;   
}

void Map::setDebug (bool value) {
    debug = true;
}