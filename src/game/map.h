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
        int mapWidth;
        int mapHeight;

        // Holds the image and a clip containing all tiles
        Image* image;
        Clip* tiles;
        vector<vector<int>> grid;
        Map(Image* image, int tileWidth, int tileHeight, int mapWidth, int mapHeight) {
            this->image = image;
            this->tileWidth = tileWidth;
            this->tileHeight = tileHeight;
            this->mapWidth = mapWidth;
            this->mapHeight = mapHeight;
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
            return mapWidth*tileWidth;
        }
        virtual int getHeight() {
            return mapHeight*tileHeight;
        }

        virtual void render(Camera* camera) {
            SDL_Rect position;
            position.w = tileWidth;
            position.h = tileHeight;
            for (size_t x = 0; x < mapWidth; x++)
            {
                for (size_t y = 0; y < mapHeight; y++)
                {
                    //grid[x][y]
                    position.x = x*tileWidth;
                    position.y = y*tileHeight;
                    if (camera->isVisible(position.x, position.y, tileWidth, tileHeight)) {
                        camera->translate(position.x, position.y, &position);
                        image->render(&tiles->getFrame(grid[x][y])->rect, &position);
                    }
                }
                
            }            
        }

};
#endif
