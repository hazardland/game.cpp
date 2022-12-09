#ifndef TEST_UNIT_H
#define TEST_UNIT_H

using namespace std;

#include <iostream>

#include <game/sprite.hpp>
#include <game/object.hpp>
#include <game/animation.hpp>
#include <game/rectangle.hpp>

#include <test_enum.hpp>

class Footman: public Object {
    public:
        int x;
        int y;
        Animation* body;
        Rectangle* health;
        Footman(Sprite* sprite) {
            // cout << "Creating footman\n";

            setSize(72, 72);

            // setPosition(x, y);
            body = new Animation(sprite, UNIT_STAND_DOWN);
            health = new Rectangle(sprite->image->renderer);
            health->setSize(getWidth()*0.4, 5);
            health->color = SDL_Color{161, 195, 69, 100};
        }
        virtual void update(uint32_t delta, Input* input) {
            //cout << "Updating footman\n";
            int clip = 0;
            if (input->keyboard->up) {
                clip += UNIT_STAND_UP;
            } else if (input->keyboard->down) {
                clip += UNIT_STAND_DOWN;
            }
            if (input->keyboard->left) {
                clip += UNIT_STAND_LEFT;
            } else if (input->keyboard->right) {
                clip += UNIT_STAND_RIGHT;
            }
            if (clip>0) {
                if (input->keyboard->space) {
                    clip += 100;
                } else {
                    clip += UNIT_MOVE;
                }
                body->play(clip);
            }
            body->update(delta);
        }
        virtual void render(Camera* camera) {
            // cout << "Rendering footman\n";
            if (camera->isVisible(x, y, getWidth(), getHeight())) {
                camera->translate(x, y, &position);
                body->render(&position);
                health->setPosition(getX()+getWidth()/2-health->getWidth()/2, getY()+10);                
                health->render(camera);
            }
        }

};
#endif
