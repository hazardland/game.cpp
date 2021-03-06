#ifndef WAR2_FOOTMAN_H
#define WAR2_FOOTMAN_H

using namespace std;
#include <iostream>

#include <game/sprite.h>
#include <game/object.h>
#include <game/animation.h>
#include <game/rectangle.h>

#include <enum.h>

class Footman: public Object {
    public:
        Animation* body;
        Rectangle* health;
        Footman(Sprite* sprite) {
            // cout << "Creating footman\n";
            setSize(72, 72);
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
            health->setPosition(getX()+getWidth()/2-health->getWidth()/2, getY()+10);
        }
        virtual void render() {
            // cout << "Rendering footman\n";
            body->render(&position);
            health->render();
        }

};
#endif
