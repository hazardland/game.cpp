#ifndef WAR2_FOOTMAN_H
#define WAR2_FOOTMAN_H

using namespace std;
#include <iostream>

#include <game/sprite.h>
#include <game/object.h>
#include <game/animation.h>

#include <enum.h>

class Footman: public Object {
    public:
        Animation* body;
        Footman(Sprite* sprite) {
            // cout << "Creating footman\n";
            setSize(72, 72);
            body = new Animation(sprite, UNIT_STAND_DOWN);
        }
        virtual void update(uint32_t delta, Input* input) {
            //cout << "Updating footman\n";
            int clip = 0;
            if (input->up) {
                clip += UNIT_STAND_UP;
            } else if (input->down) {
                clip += UNIT_STAND_DOWN;
            }
            if (input->left) {
                clip += UNIT_STAND_LEFT;
            } else if (input->right) {
                clip += UNIT_STAND_RIGHT;
            }
            if (clip>0) {
                if (input->space) {
                    clip += 100;
                } else {
                    clip += UNIT_MOVE;
                }
                body->play(clip);
            }
            body->update(delta);
        }
        virtual void render() {
            // cout << "Rendering footman\n";
            body->render(&position);
        }

};
#endif
