#ifndef TEST_SCENE_H
#define TEST_SCENE_H

using namespace std;
#include <iostream>
#include <map>
#include <vector>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <game/scene.h>
#include <game/text.h>

#include <enum.h>
#include <footman.h>
#include <selectbox.h>

class TestScene : public Scene {

    using Scene::Scene;
    Footman* footman; 
    TTF_Font* font;
    Text* fps;
    Text* cursor;
    Selectbox* selectbox;

    public:

        const int FOOTMAN_BLUE = 1;

        virtual void prepare() {
            font = TTF_OpenFont("assets/fonts/titillium.ttf", 64);
            if (font==NULL){
                printf("Failed to load font: %s", SDL_GetError());
            }

            fps = new Text(renderer, font);
            cursor = new Text(renderer, font);

            Sprite* sprite = new Sprite(new Image(renderer, "assets/sprites/footman.png", RED, BLUE),
                                        72,
                                        72,
                                        100,
                                        true);

            sprite->add (UNIT_MOVE_UP, 1, 2, 4, true, false);
            sprite->add (UNIT_MOVE_DOWN, 5, 2, 4, false, false);
            sprite->add (UNIT_MOVE_RIGHT, 3, 2, 4, false, false);
            sprite->add (UNIT_MOVE_LEFT, 3, 2, 4, true, false);
            sprite->add (UNIT_MOVE_UP_RIGHT, 2, 2, 4, false, false);
            sprite->add (UNIT_MOVE_DOWN_RIGHT, 4, 2, 4, false, false);
            sprite->add (UNIT_MOVE_UP_LEFT, 2, 2, 4, true, false);
            sprite->add (UNIT_MOVE_DOWN_LEFT, 4, 1, 4, true, false);
            sprite->add (UNIT_STAND_UP, 1, 1, 1, false, false);
            sprite->add (UNIT_STAND_DOWN, 5, 1, 1, false, false);
            sprite->add (UNIT_STAND_RIGHT, 3, 1, 1, false, false);
            sprite->add (UNIT_STAND_LEFT, 3, 1, 1, true, false);
            sprite->add (UNIT_STAND_UP_RIGHT, 2, 1, 1, false, false);
            sprite->add (UNIT_STAND_DOWN_RIGHT, 4, 1, 1, false, false);
            sprite->add (UNIT_STAND_UP_LEFT, 2, 1, 1, true, false);
            sprite->add (UNIT_STAND_DOWN_LEFT, 4, 1, 1, true, false);
            sprite->add (UNIT_ATTACK_UP, 1, 6, 4, false, false);
            sprite->add (UNIT_ATTACK_DOWN, 5, 6, 4, false, false);
            sprite->add (UNIT_ATTACK_RIGHT, 3, 6, 4, false, false);
            sprite->add (UNIT_ATTACK_LEFT, 3, 6, 4, true, false);
            sprite->add (UNIT_ATTACK_UP_RIGHT, 2, 6, 4, false, false);
            sprite->add (UNIT_ATTACK_DOWN_RIGHT, 4, 6, 4, false, false);
            sprite->add (UNIT_ATTACK_UP_LEFT, 2, 6, 4, true, false);
            sprite->add (UNIT_ATTACK_DOWN_LEFT, 4, 6, 4, true, false);

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

        virtual void update(Clock *clock, Input* input, Camera* camera) {
            // footman->setPosition(x, y);

            fps->setText(to_string(clock->fps));

            cursor->setPosition(input->mouse->x, input->mouse->y);
            cursor->setText(to_string(input->mouse->x)+", "+to_string(input->mouse->y));
            
            if (input->keyboard->down) {
                camera->y += 5;
            }
            if (input->keyboard->up) {
                camera->y -= 5;
            }
            if (input->keyboard->right) {
                camera->x += 5;
            }
            if (input->keyboard->left) {
                camera->x -= 5;
            }

            Scene::update(clock, input, camera);
        }

};

#endif
