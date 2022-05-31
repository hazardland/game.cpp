#ifndef WAR2_SCENE_H
#define WAR2_SCENE_H

using namespace std;
#include <iostream>
#include <map>
#include <vector>

#include <SDL2/SDL_image.h>

#include <game/scene.h>

#include <enum.h>
#include <footman.h>

class Game : public Scene {

    using Scene::Scene;

    public:

        const int FOOTMAN_BLUE = 1;

        virtual void prepare() {
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
                    footman->setPosition(x*footman->getWidth()/2, y*footman->getHeight()/2);
                    units.push_back(footman);
                }
            }


        }

};

#endif
