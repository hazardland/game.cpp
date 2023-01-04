#ifndef TEST_FOOTMAN_H
#define TEST_FOOTMAN_H

using namespace std;

#include <iostream>

#include <game/sprite.hpp>
#include <game/object.hpp>
#include <game/animation.hpp>
#include <game/rectangle.hpp>

#include <examples/enum.h>

class Footman: public Object {
    public:
        Animation* body;
        Rectangle* outline;
        int speed = 20;
        int mode = STAND;
        int modeX = RIGHT;
        int modeY = DOWN;
        Footman(Sprite* sprite) {
            // cout << "Creating footman\n";

            setSize(72, 72);

            // setPosition(x, y);
            body = new Animation(sprite, mode+modeX+modeY);
            body->pause = 1000;
            outline = new Rectangle(sprite->image->renderer);
            outline->setSize(getWidth(), getHeight());
            outline->border = SDL_Color{161, 195, 69, 100};
        }
        virtual void rotate(float x, float y) {
            if (x!=0 || y!=0) {
                if (x!=0) {
                    if (x<0) {
                        modeX = LEFT;
                    } else if (x>0) {
                        modeX = RIGHT;
                    }
                } else {
                    modeX = 0;
                }
                if (y!=0) {
                    if (y<0) {
                        modeY = UP;
                    } else if (y>0) {
                        modeY = DOWN;
                    }
                } else {
                    modeY = 0;
                }
            }
        }
        virtual void move(int delta, float x, float y) {
            position.x += (x*delta)/(100/speed); 
            position.y += (y*delta)/(100/speed);       
        }
        virtual void update(State* state) {
            //cout << "Updating footman\n";
            Keyboard* key = state->event->keyboard;
            float x = 0;
            float y = 0;
            if (key->up) {
                // cout << "up\n";
                y = -1;
            } else if (key->down) {
                y = 1;
                // cout << "down\n";
            }
            if (key->left) {
                // cout << "left\n";
                x = -1;
            } else if (key->right) {
                x = 1;
                // cout << "right\n";
            }
            rotate(x, y);
            if (key->space) {
                mode = ATTACK;
            } else {
                if (x==0 && y==0) {
                    mode = STAND;
                } else {
                    mode = WALK;
                    move(state->clock->delta, x, y);
                }
            }
            body->play(mode+modeX+modeY);
            body->update(state->clock->delta);
        }
        virtual void render(State* state) {
            Camera* camera = state->camera;
            if (camera->isVisible(getPosition())) {
                if (getY()-camera->y<200) {
                    camera->y--;
                }
                if (getX()-camera->x<200) {
                    camera->x--;
                }
                if (camera->y+camera->height-getY()<200) {
                    camera->y++;
                }
                if (camera->x+camera->width-getX()<200) {
                    camera->x++;
                }
                SDL_FRect* location = camera->translate(getPosition());
                body->render(location);
                //setPosition(location->x, location->y);
                outline->setPosition(location->x, location->y);
                outline->render(state);                
            }
        }

};
#endif
