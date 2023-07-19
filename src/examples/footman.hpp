#ifndef TEST_FOOTMAN_H
#define TEST_FOOTMAN_H

using namespace std;
#include <iostream>

#include <game/sprite.hpp>
#include <game/unit.hpp>
#include <game/animation.hpp>
#include <game/rectangle.hpp>
#include <game/map.hpp>
#include <game/minimap.hpp>

#include <examples/enum.h>

/*
    The object is pretty complicated
    I need to come up with the way to standartize it
    The code of framework envovleved a lot

*/
class Footman: public Unit {
    public:
        Animation* body;
        Position* outline;
        int maxSpeed = 100;
        int speed = 99;
        int mode = STAND;
        int modeX = RIGHT;
        int modeY = DOWN;
        int cameraEdge = 100;
        int cameraStep = 1;
        bool cameraScroll = true; 

        Footman(Sprite* sprite) {
            // cout << "Creating footman\n";
            setLayer(1);
            setSize(24, 24);
            // setPosition(64, 64);
            // setPosition(x, y);
            body = new Animation(sprite, mode+modeX+modeY);
            body->pause = 0;
            renderPosition = createPosition(-24, -24, 72, 72);
            outline = createPosition(-12, -12, 48, 48);
        }
        virtual Uint32 getMinimapColor(SDL_PixelFormat* format) {
            return SDL_MapRGBA(format, 255, 255, 0, 255);
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
        // Speed concept needs to be solved
        // Here for example we have maximum speed 100
        virtual void move(int delta, float x, float y) {
            addPosition ((x*delta)/(maxSpeed+1-speed), (y*delta)/(maxSpeed+1-speed));
        }

        virtual void update(State* state) override {
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
                    cameraFollow(state->camera);
                }
            }
            body->play(mode+modeX+modeY);
            body->update(state->clock->delta);
        }

        virtual void cameraFollow(Camera* camera) {
            if (!isSelected()) {
                return;
            }
            if (getRenderY() - camera->y < cameraEdge) {
                camera->y -= cameraEdge - (getRenderY() - camera->y);
            }
            if (getRenderX() - camera->x < cameraEdge) {
                camera->x -= cameraEdge - (getRenderX() - camera->x);
            }
            if (camera->y + camera->height - getRenderY() - getRenderWidth() < cameraEdge) {
                camera->y += cameraEdge - (camera->y + camera->height - getRenderY() - getRenderWidth());
            }
            if (camera->x + camera->width - getRenderX() - getRenderHeight() < cameraEdge) {
                camera->x += cameraEdge - (camera->x + camera->width - getRenderX() - getRenderHeight());
            }
        }

        virtual void render(State* state) {
            // cout << "Rendering footman\n";
            Camera* camera = state->camera;
            if (camera->isVisible(getRenderPosition())) {
                // cout << "Footman  x:" << location->x << ", y:" << location->y  << ", w:" << location->w  << ", h:" << location->h << "\n";
                // SDL_FRect* location = camera->translate(getRenderPosition());
                drawPosition(state);
                // renderPosition->draw(state);
                // outline->draw(state);
                body->render(camera->translate(getRenderPosition()));
            }
        }

        ~Footman() {
            delete outline;
        }

};
#endif
