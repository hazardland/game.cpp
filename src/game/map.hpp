#ifndef GAME_MAP
#define GAME_MAP

using namespace std;

#include <stdio.h>

#include <game/object.hpp>
#include <game/image.hpp>
#include <game/clip.hpp>
#include <game/state.hpp>

/**
 * @brief Map
 * 
 */
class Map: public Object {

    public:
        int tileWidth;
        int tileHeight;
        float tileZoom;
        int mapWidth;
        int mapHeight;

        // Holds the image and a clip containing all tiles
        Image* image;
        Clip* tiles;
        vector<vector<int>> grid;
        Map(Image* image, int tileWidth, int tileHeight, int mapWidth, int mapHeight, float tileZoom=1) {
            this->image = image;
            this->tileWidth = tileWidth;
            this->tileHeight = tileHeight;
            this->mapWidth = mapWidth;
            this->mapHeight = mapHeight;
            this->tileZoom = tileZoom;
            tiles = new Clip(image, 
                             tileWidth, 
                             tileHeight, 
                             1, 1, 
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
            return mapWidth*tileWidth*tileZoom;
        }
        virtual int getHeight() {
            return mapHeight*tileHeight*tileZoom;
        }

        virtual void render(State* state) {
            SDL_Rect position;
            position.w = tileWidth*tileZoom;
            position.h = tileHeight*tileZoom;
            for (size_t x = 0; x < mapWidth; x++)
            {
                for (size_t y = 0; y < mapHeight; y++)
                {
                    //grid[x][y]
                    // position.x = x*tileWidth*tileZoom;
                    // position.y = y*tileHeight*tileZoom;
                    if (state->camera->isVisible(x*tileWidth*tileZoom, y*tileHeight*tileZoom, &position)) {
                        state->camera->translate(x*tileWidth*tileZoom, y*tileHeight*tileZoom, &position);
                        // printf("%d ", tiles->getFrame(grid[x][y])->rect.x);
                        image->render(&tiles->getFrame(grid[x][y])->rect, &position);
                        SDL_SetRenderDrawColor(image->renderer, 0, 0, 0, 255);
                        SDL_RenderDrawRect(image->renderer, &position);
                        SDL_SetRenderDrawColor(image->renderer, 0, 0, 0, 0);

                    }
                }
                
            }            
        }

};
#endif
