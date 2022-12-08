#ifndef GAME_MAP_H
#define GAME_MAP_H

using namespace std;
#include <stdio.h>

#include <game/object.h>
#include <game/image.h>
#include <game/clip.h>

/**
 * @brief Map
 * 
 */
class Map: public Object {

    public:
        int tileWidth;
        int tileHeight;
        int tileScale;
        int mapWidth;
        int mapHeight;

        // Holds the image and a clip containing all tiles
        Image* image;
        Clip* tiles;
        vector<vector<int>> grid;
        Map(Image* image, int tileWidth, int tileHeight, int mapWidth, int mapHeight, int tileScale=1) {
            this->image = image;
            this->tileWidth = tileWidth;
            this->tileHeight = tileHeight;
            this->mapWidth = mapWidth;
            this->mapHeight = mapHeight;
            this->tileScale = tileScale;
            tiles = new Clip(image, 
                             tileWidth, 
                             tileHeight, 
                             0, 0, 
                             (image->getWidth()/tileWidth)*(image->getHeight()/tileHeight));
            for (size_t i = 0; i < mapWidth; i++)
            {
                grid.push_back(vector<int>());
                for (size_t j = 0; j < mapHeight; j++)
                {
                    grid[i].push_back(0);                    
                }
            }
                        
        }
        virtual int getWidth() {
            return mapWidth*tileWidth*tileScale;
        }
        virtual int getHeight() {
            return mapHeight*tileHeight*tileScale;
        }

        virtual void render(Camera* camera) {
            SDL_Rect position;
            position.w = tileWidth*tileScale;
            position.h = tileHeight*tileScale;
            for (size_t x = 0; x < mapWidth; x++)
            {
                for (size_t y = 0; y < mapHeight; y++)
                {
                    //grid[x][y]
                    position.x = x*tileWidth*tileScale;
                    position.y = y*tileHeight*tileScale;
                    if (camera->isVisible(position.x, position.y, tileWidth*tileScale, tileHeight*tileScale)) {
                        camera->translate(position.x, position.y, &position);
                        image->render(&tiles->getFrame(grid[x][y])->rect, &position);
                    }
                }
                
            }            
        }

};
#endif
