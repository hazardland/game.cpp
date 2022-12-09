#ifndef TEST_SCENE_H
#define TEST_SCENE_H

using namespace std;
#include <iostream>
#include <map>
#include <vector>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <game/scene.hpp>
#include <game/text.hpp>
#include <game/map.hpp>
#include <game/state.hpp>

#include <test_enum.hpp>
#include <test_unit.hpp>
#include <test_selectbox.hpp>



class TestScene : public Scene {

    using Scene::Scene;
    
    Map* map;
    
    Footman* footman; 
    TTF_Font* font;
    Text* fps;
    Text* cursor;
    Selectbox* selectbox;

    public:

        int random(int min, int max) {
            return rand()%(max-min + 1) + min;
        }

        const int FOOTMAN_BLUE = 1;

        virtual void prepare() {
            font = TTF_OpenFont("assets/fonts/titillium.ttf", 64);
            if (font==NULL){
                printf("Failed to load font: %s", SDL_GetError());
            }

            map = new Map(new Image(renderer, "assets/sprites/tileset_compressed.png"), 16, 16, 500, 500, 4);
            int mapFrameCount = map->tiles->getFrameCount();
            printf("map frame count %d\n", mapFrameCount);
            for (size_t x = 0; x < map->mapWidth; x++)
            {
                for (size_t y = 0; y < map->mapHeight; y++)
                {
                    map->grid[x][y] = random(0, mapFrameCount-1);
                }
                
            }
            
            objects.push_back(map);

            fps = new Text(renderer, font);
            cursor = new Text(renderer, font);

            Sprite* sprite = new Sprite(new Image(renderer, "assets/sprites/footman.png", RED, BLUE),
                                        72,
                                        72,
                                        100,
                                        true);

            sprite->addClip (UNIT_MOVE_UP, 1, 2, 4, true, false);
            sprite->addClip (UNIT_MOVE_DOWN, 5, 2, 4, false, false);
            sprite->addClip (UNIT_MOVE_RIGHT, 3, 2, 4, false, false);
            sprite->addClip (UNIT_MOVE_LEFT, 3, 2, 4, true, false);
            sprite->addClip (UNIT_MOVE_UP_RIGHT, 2, 2, 4, false, false);
            sprite->addClip (UNIT_MOVE_DOWN_RIGHT, 4, 2, 4, false, false);
            sprite->addClip (UNIT_MOVE_UP_LEFT, 2, 2, 4, true, false);
            sprite->addClip (UNIT_MOVE_DOWN_LEFT, 4, 1, 4, true, false);
            sprite->addClip (UNIT_STAND_UP, 1, 1, 1, false, false);
            sprite->addClip (UNIT_STAND_DOWN, 5, 1, 1, false, false);
            sprite->addClip (UNIT_STAND_RIGHT, 3, 1, 1, false, false);
            sprite->addClip (UNIT_STAND_LEFT, 3, 1, 1, true, false);
            sprite->addClip (UNIT_STAND_UP_RIGHT, 2, 1, 1, false, false);
            sprite->addClip (UNIT_STAND_DOWN_RIGHT, 4, 1, 1, false, false);
            sprite->addClip (UNIT_STAND_UP_LEFT, 2, 1, 1, true, false);
            sprite->addClip (UNIT_STAND_DOWN_LEFT, 4, 1, 1, true, false);
            sprite->addClip (UNIT_ATTACK_UP, 1, 6, 4, false, false);
            sprite->addClip (UNIT_ATTACK_DOWN, 5, 6, 4, false, false);
            sprite->addClip (UNIT_ATTACK_RIGHT, 3, 6, 4, false, false);
            sprite->addClip (UNIT_ATTACK_LEFT, 3, 6, 4, true, false);
            sprite->addClip (UNIT_ATTACK_UP_RIGHT, 2, 6, 4, false, false);
            sprite->addClip (UNIT_ATTACK_DOWN_RIGHT, 4, 6, 4, false, false);
            sprite->addClip (UNIT_ATTACK_UP_LEFT, 2, 6, 4, true, false);
            sprite->addClip (UNIT_ATTACK_DOWN_LEFT, 4, 6, 4, true, false);

            sprites[FOOTMAN_BLUE] = sprite;

            for (int x = 0; x <= 50; ++x)
            {
                for (int y = 0; y <= 25; ++y)
                {                    
                    Footman* footman = new Footman(sprites[FOOTMAN_BLUE]);
                    footman->x = x*footman->getWidth()/2;
                    footman->y = y*footman->getHeight()/2;                    
                    objects.push_back(footman);
                }
            }

            // footman = new Footman(sprites[FOOTMAN_BLUE]);
            // footman->setPosition(100, 100);
            // objects.push_back(footman);

            selectbox = new Selectbox(renderer);

            objects.push_back(fps);
            objects.push_back(cursor);
            objects.push_back(selectbox);

            printf("Object count: %d", objects.size());

        }

        virtual void update(State* state) {
            // footman->setPosition(x, y);

            fps->setText((state->clock->fps>=100?"":" ") + to_string(state->clock->fps));

            cursor->setPosition(state->event->mouse->x, state->event->mouse->y);
            cursor->setText(to_string(state->event->mouse->x)+", "+to_string(state->event->mouse->y));
            
            if (state->event->keyboard->down) {
                state->camera->y += 5;
            }
            if (state->event->keyboard->up) {
                state->camera->y -= 5;
            }
            if (state->event->keyboard->right) {
                state->camera->x += 5;
            }
            if (state->event->keyboard->left) {
                state->camera->x -= 5;
            }

            Scene::update(state);
        }

};

#endif
