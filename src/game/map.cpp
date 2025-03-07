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
         float tileScale, 
         int layerCount, 
         Text* text) {

    this->image = image;
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    this->tilesPerWidth = tilesPerWidth;
    this->tilesPerHeight = tilesPerHeight;
    this->tileScale = tileScale;
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
    this->text = text;
    this->text->color = SDL_Color(0, 0, 0);

}


float Map::getWidth() {
    return tilesPerWidth*tileWidth*tileScale;
}

float Map::getHeight() {
    return tilesPerHeight*tileHeight*tileScale;
}

void Map::render(State* state) {

    Camera* camera = state->camera;

    // Choose renderable tiles
    // @todo Code can be optimized furzer
    int xTileFrom = (camera->x / (tileWidth*tileScale));
    int xTileTo = (camera->width / (tileWidth*tileScale)) + xTileFrom + 2;
    int yTileFrom = (camera->y / (tileHeight*tileScale));
    int yTileTo = (camera->height / (tileHeight*tileScale)) + yTileFrom + 2;
    if (xTileFrom<0) xTileFrom = 0;
    if (yTileFrom<0) yTileFrom = 0;
    if (xTileTo>tilesPerWidth) xTileTo = tilesPerWidth;
    if (yTileTo>tilesPerHeight) yTileTo = tilesPerHeight;
    //printf("map render region: %d,%d x %d,%d\n", xTileFrom, yTileFrom, xTileTo, yTileTo);

    SDL_FRect location;
    location.w = tileWidth*tileScale;
    location.h = tileHeight*tileScale;
    SDL_FRect* position;

    for (int x = xTileFrom; x < xTileTo; x++)
    {
        for (int y = yTileFrom; y < yTileTo; y++)
        {
            //grid[x][y]
            location.x = x*tileWidth*tileScale;
            location.y = y*tileHeight*tileScale;
            if (state->camera->isVisible(&location)) {
                position = state->camera->translate(&location);
                image->render(grid[x][y]->rect, position);
                
                if (debug){

                    // printf("%d ", clip->getFrame(grid[x][y])->rect.x);
                    SDL_SetRenderDrawColor(image->renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRectF(image->renderer, position);
                    SDL_SetRenderDrawColor(image->renderer, 0, 0, 0, 0);

                    Cell* cell = grid[x][y]; 
                    int type = cell->terrain->id;
                    
                    int borders[] = {
                        type, type, type, // 0 1 2 
                        type, type, type, // 3 4 5
                        type, type, type  // 6 7 8
                    };
                    // int borders[] = {
                    //     9, 9, 9, // 0 1 2 
                    //     9, 9, 9, // 3 4 5
                    //     9, 9, 9  // 6 7 8
                    // };
                    
                    int corners[] = {
                        0, 0, // 0 1
                        0, 0  // 2 3
                    };

                    if (x!=0 && y!=0){
                        borders[0] = this->grid[x-1][y-1]->terrain->id;
                    }
                    if (y!=0){
                        borders[1] = this->grid[x  ][y-1]->terrain->id;
                    }
                    if (y!=0 && x<tilesPerWidth-1) {
                        borders[2] = this->grid[x+1][y-1]->terrain->id;
                    }
                    if (x!=0) {
                        borders[3] = this->grid[x-1][y  ]->terrain->id;
                    }
                    if (x<tilesPerWidth-1) {
                        borders[5] = this->grid[x+1][y  ]->terrain->id;
                    }
                    if (x!=0 && y<tilesPerHeight-1) {
                        borders[6] = this->grid[x-1][y+1]->terrain->id;
                    }
                    if (y<tilesPerHeight-1){
                        borders[7] = this->grid[x  ][y+1]->terrain->id;
                    }
                    if (x<tilesPerWidth-1 && y<tilesPerHeight-1) {
                        borders[8] = this->grid[x+1][y+1]->terrain->id;
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

                    text->setText(std::to_string(borders[0])); //+"tl"
                    text->setPosition(position->x+tileScale, position->y+tileScale);
                    // if (corner==0) text->setColor(255,255,255);
                    // else text->setColor(0,0,0);
                    text->render(state);

                    text->setText(std::to_string(borders[1])); //+"tm"
                    text->setPosition(position->x+position->w/2-text->getWidth()/2, position->y+tileScale);
                    text->render(state);

                    text->setText(std::to_string(borders[2])); //+"tr"
                    text->setPosition(position->x+position->w-text->getWidth()-tileScale, position->y+tileScale);
                    text->render(state);


                    text->setText(std::to_string(borders[3])); //+"ml"
                    text->setPosition(position->x+tileScale, position->y+position->h/2-text->getHeight()/2);
                    text->render(state);


                    text->setText(std::to_string(borders[4])); //+"mid"
                    text->setPosition(position->x+position->w/2-text->getWidth()/2, position->y+position->h/2-text->getHeight()/2);
                    text->render(state);

                    text->setText(std::to_string(borders[5])); //+"mr"
                    text->setPosition(position->x+position->w-text->getWidth()-tileScale, position->y+position->h/2-text->getHeight()/2);
                    text->render(state);


                    text->setText(std::to_string(borders[6])); //+"bl"
                    text->setPosition(position->x+tileScale, position->y+position->h-text->getHeight()-tileScale);
                    text->render(state);

                    text->setText(std::to_string(borders[7])); //+"bm"
                    text->setPosition(position->x+position->w/2-text->getWidth()/2, position->y+position->h-text->getHeight()-tileScale);
                    text->render(state);


                    text->setText(std::to_string(borders[8])); //+"br"
                    text->setPosition(position->x+position->w-text->getWidth()-tileScale, position->y+position->h-text->getHeight()-tileScale);
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

// // Implement setMinimap method
// void Map::setMinimap(Minimap* minimap) {
//     // Implementation details
// }

// // Implement getTile method
// int Map::getTile(string key) {
//     // Implementation details
// }

// // // Implement import method
// // void Map::import(vector<vector<int>> data) {
// //     // Implementation details
// // }

// // Implement generate1 method
// void Map::generate1(int seed, float intensity, vector<float> ranges) {
//     // Implementation details
// }

// // Implement generate2 method
// void Map::generate2(int seed, float intensity, vector<float> ranges) {
//     // Implementation details
// }

// // Implement fillMap2 method
// void Map::fillMap2() {
//     // Implementation details
// }

// // Implement getTile2 method
// int Map::getTile2(int x, int y) {
//     // Implementation details
// }

// // Implement random method
// int Map::random(int min, int max) {
//     // Implementation details
// }

void Map::setMinimap(Minimap* minimap) {
    this->minimap = minimap;
    minimap->setMapData(grid, tileWidth, tileHeight, tileScale);
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
}

void Map::generate1(int seed, 
                float intensity,
                std::vector<float> ranges) {

    OpenSimplexNoise::Noise noise{seed};
    for (int x=0; x<tilesPerWidth; x++) {
        for (int y=0; y<tilesPerHeight; y++)
        {
            // alpha = (noise.eval(x*0.01, y*0.01) + 1) / 2.0  * 255.0;
            // minimap->setPixel(x, y, 255, 255, 255, alpha);       
            float depth = (noise.eval(x*intensity, y*intensity) + 1) / 2.0;
            for (int terrain=0; terrain<terrains.size(); terrain++){
                if (depth<=ranges[terrain]) {
                    setTerrain(x, y, terrain);
                    break;
                }
            }
        }
    }
    //map->grid = this->grid;
}

void Map::generate2(
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

}  


void Map::fillMap2() {
    
    for (int x=0; x<tilesPerWidth; x++) {
        for (int y=0; y<tilesPerHeight; y++) {
            grid[x][y]->tile = getTile2(x, y);
            grid[x][y]->rect = &clip->getFrame(grid[x][y]->tile)->rect;
        }
    }
    printf("filled map 2\n");
}

int Map::getTile2(int x, int y) {
    // Get corner sum
    Cell* cell = this->grid[x][y]; 
    int type = cell->terrain->id;
    // Terrain* terrain = cell->terrain;
    if (type==0) {
        return getTile("0.0.0.0");
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

    // int borders[] = {
    //     1, 1, 1, // 0 1 2 
    //     1, 1, 1, // 3 4 5
    //     1, 1, 1  // 6 7 8
    // };
    // int corners[] = {
    //     0, 0, // 0 1
    //     0, 0  // 2 3
    // };
    
    std::string result;
    if (corner==0) {
        result = 
        std::to_string(borders[0]) + "." +
        std::to_string(borders[1]) + "." +
        std::to_string(borders[3]) + "." +
        std::to_string(borders[4]);
    }
    if (corner==1) {
        result = 
        std::to_string(borders[1]) + "." +
        std::to_string(borders[2]) + "." +
        std::to_string(borders[4]) + "." +
        std::to_string(borders[5]);

    }
    if (corner==2) { 
        result = 
        std::to_string(borders[3]) + "." +
        std::to_string(borders[4]) + "." +
        std::to_string(borders[6]) + "." +
        std::to_string(borders[7]);

    }
    if (corner==3) {
        result = 
        std::to_string(borders[4]) + "." +
        std::to_string(borders[5]) + "." +
        std::to_string(borders[7]) + "." +
        std::to_string(borders[8]);        
    }

    // printf("-- x: %d y: %d c: %d\n%d %d %d\n%d %d %d\n%d %d %d\n",
    // x, y, corner, borders[0],borders[1], borders[2], borders[3], borders[4], borders[5], borders[6], borders[7], borders[8]        
    // );
    
    // Mark bad tile
    // if (result=="1.0.0.1") {
    //     this->setPixel(x, y, 0, 0, 0, 255);
    // }

    if (result.compare("1.1.1.1")==0) {
        return getTile("1.1.1.1");
    }
    return getTile(result);
}

int Map::random(int min, int max) {
    return rand() % (max-min+1) + min;   
}
