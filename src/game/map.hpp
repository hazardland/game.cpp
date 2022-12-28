#ifndef GAME_MAP
#define GAME_MAP

using namespace std;

#include <stdio.h>

#include <game/object.hpp>
#include <game/image.hpp>
#include <game/clip.hpp>
#include <game/state.hpp>
#include <game/text.hpp>

/**
 * @brief Map
 * 
 */
class Map: public Object {

    public:
        int tileWidth;
        int tileHeight;
        float scale;
        int width;
        int height;
        bool debug = true;

        // Holds the image and a clip containing all tiles
        Image* image;
        Clip* clip;

        vector<vector<int>> tiles;
        Text* text;
        vector<vector<int>> terrain;


        Map(Image* image, 
            int tileWidth, 
            int tileHeight, 
            int width, 
            int height,
            float scale=1,
            Text* text = NULL
        ) {
            this->image = image;
            this->tileWidth = tileWidth;
            this->tileHeight = tileHeight;
            this->width = width;
            this->height = height;
            this->scale = scale;
            clip = new Clip(image, 
                             tileWidth, 
                             tileHeight, 
                             1, 1, 
                             (image->getWidth()/tileWidth)*(image->getHeight()/tileHeight));
            for (int i = 0; i < width; i++)
            {
                tiles.push_back(vector<int>());
                for (int j = 0; j < height; j++)
                {
                    tiles[i].push_back(0);                    
                }
            }
            this->text = text;
            this->text->color = SDL_Color(0, 0, 0);
        }
        virtual int getWidth() {
            return width*tileWidth*scale;
        }
        virtual int getHeight() {
            return height*tileHeight*scale;
        }

        virtual void render(State* state) {
            SDL_Rect location;
            location.w = tileWidth*scale;
            location.h = tileHeight*scale;
            SDL_Rect* position;
            // I think we do not need to cycle through all tiles to check if it is visible
            // We simply need to calculate what region could be visible
            // For this we take camera region
            int x_from = (state->camera->x/(tileWidth*scale));
            int x_to = (state->camera->width/(tileWidth*scale)) + x_from + 2;
            int y_from = (state->camera->y/(tileHeight*scale));
            int y_to = (state->camera->height/(tileHeight*scale)) + y_from + 2;
            if (x_from<0) x_from = 0;
            if (y_from<0) y_from = 0;
            if (x_to>width) x_to = width;
            if (y_to>height) y_to = height;
            //printf("map render region: %d,%d x %d,%d\n", x_from, y_from, x_to, y_to);
            for (int x = x_from; x < x_to; x++)
            {
                for (int y = y_from; y < y_to; y++)
                {
                    //grid[x][y]
                    location.x = x*tileWidth*scale;
                    location.y = y*tileHeight*scale;
                    if (state->camera->isVisible(&location)) {
                        position = state->camera->translate(&location);
                        image->render(&clip->getFrame(tiles[x][y])->rect, position);
                        
                        if (debug){

                            // printf("%d ", clip->getFrame(grid[x][y])->rect.x);
                            SDL_SetRenderDrawColor(image->renderer, 0, 0, 0, 255);
                            SDL_RenderDrawRect(image->renderer, position);
                            SDL_SetRenderDrawColor(image->renderer, 0, 0, 0, 0);

                            int borders[] = {
                                1, 1, 1, // 0 1 2 
                                1, 1, 1, // 3 4 5
                                1, 1, 1  // 6 7 8
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
                            int type = terrain[x][y];
                            if (x!=0 && y!=0){
                                borders[0] = terrain[x-1][y-1]!=type ? 0 : 1;
                            }
                            if (y!=0){
                                borders[1] = terrain[x  ][y-1]!=type ? 0 : 1;
                            }
                            if (y!=0 && x<width-1) {
                                borders[2] = terrain[x+1][y-1]!=type ? 0 : 1;
                            }
                            if (x!=0) {
                                borders[3] = terrain[x-1][y  ]!=type ? 0 : 1;
                            }
                            if (x<width-1) {
                                borders[5] = terrain[x+1][y  ]!=type ? 0 : 1;
                            }
                            if (x!=0 && y<height-1) {
                                borders[6] = terrain[x-1][y+1]!=type ? 0 : 1;
                            }
                            if (y<height-1){
                                borders[7] = terrain[x  ][y+1]!=type ? 0 : 1;
                            }
                            if (x<width-1 && y<height-1) {
                                borders[8] = terrain[x+1][y+1]!=type ? 0 : 1;
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

                            text->setText(to_string(borders[0])); //+"tl"
                            text->setPosition(position->x+scale, position->y+scale);
                            // if (corner==0) text->setColor(255,255,255);
                            // else text->setColor(0,0,0);
                            text->render(state);

                            text->setText(to_string(borders[1])); //+"tm"
                            text->setPosition(position->x+position->w/2-text->getWidth()/2, position->y+scale);
                            text->render(state);

                            text->setText(to_string(borders[2])); //+"tr"
                            text->setPosition(position->x+position->w-text->getWidth()-scale, position->y+scale);
                            text->render(state);


                            text->setText(to_string(borders[3])); //+"ml"
                            text->setPosition(position->x+scale, position->y+position->h/2-text->getHeight()/2);
                            text->render(state);


                            text->setText(to_string(borders[4])); //+"mid"
                            text->setPosition(position->x+position->w/2-text->getWidth()/2, position->y+position->h/2-text->getHeight()/2);
                            text->render(state);

                            text->setText(to_string(borders[5])); //+"mr"
                            text->setPosition(position->x+position->w-text->getWidth()-scale, position->y+position->h/2-text->getHeight()/2);
                            text->render(state);


                            text->setText(to_string(borders[6])); //+"bl"
                            text->setPosition(position->x+scale, position->y+position->h-text->getHeight()-scale);
                            text->render(state);

                            text->setText(to_string(borders[7])); //+"bm"
                            text->setPosition(position->x+position->w/2-text->getWidth()/2, position->y+position->h-text->getHeight()-scale);
                            text->render(state);


                            text->setText(to_string(borders[8])); //+"br"
                            text->setPosition(position->x+position->w-text->getWidth()-scale, position->y+position->h-text->getHeight()-scale);
                            text->render(state);
                        }


                    }
                }
                
            }            
        }

};
#endif
