#ifndef TEST_FOOTMAN_H
#define TEST_FOOTMAN_H

#include <iostream>

#include <game/unit.h>
#include <game/animation.h>
#include <game/position.h>
#include <game/state.h>
#include <game/event.h>
#include <game/camera.h>
#include <game/clock.h>

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
        int mode = IDLE;
        int modeX = RIGHT;
        int modeY = DOWN;
        int cameraEdge = 100;
        int cameraStep = 1;
        bool cameraScroll = true; 

        Footman(Sprite* sprite) {
            position = new Position(0, 0, 64, 46);
            // cout << "Creating footman\n";
            setLayer(1);
            allowTerrains({1,2});
            setSize(24, 24);
            // setPosition(64, 64);
            // setPosition(x, y);
            body = new Animation(sprite, mode+modeX+modeY);
            body->pause = 0;
            renderPosition = createChildPosition(-24, -24, 72, 72);
            // outline = createChildPosition(-12, -12, 48, 48);
        }
        virtual Uint32 getMinimapColor(SDL_PixelFormat* format) {
            return SDL_MapRGBA(format, 255, 255, 0, 255);
        }
        virtual void rotate(float directionX, float directionY) {
            if (directionX!=0 || directionY!=0) {
                if (directionX!=0) {
                    if (directionX<0) {
                        modeX = LEFT;
                    } else if (directionX>0) {
                        modeX = RIGHT;
                    }
                } else {
                    modeX = 0;
                }
                if (directionY!=0) {
                    if (directionY<0) {
                        modeY = UP;
                    } else if (directionY>0) {
                        modeY = DOWN;
                    }
                } else {
                    modeY = 0;
                }
            }
        }
        // Speed concept needs to be solved
        // Here for example we have maximum speed 100
        virtual void move(int deltaTime, float directionX, float directionY) {
            addPosition ((directionX*deltaTime)/(maxSpeed+1-speed), (directionY*deltaTime)/(maxSpeed+1-speed));
        }

        virtual void update(State* state) override {
            //cout << "Updating footman\n";
            Keyboard* key = state->event->keyboard;
            float directionX = 0;
            float directionY = 0;
            if (key->up) {
                // cout << "up\n";
                directionY = -1;
            } else if (key->down) {
                directionY = 1;
                // cout << "down\n";
            }
            if (key->left) {
                // cout << "left\n";
                directionX = -1;
            } else if (key->right) {
                directionX = 1;
                // cout << "right\n";
            }
            rotate(directionX, directionY);
            if (key->space) {
                mode = ATTACK;
            } else {
                if (directionX==0 && directionY==0) {
                    mode = IDLE;
                } else {
                    mode = MOVE;
                    move(state->clock->delta, directionX, directionY);
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
            SDL_FRect* renderRect = getRenderPosition();
            if (renderRect->y - camera->y < cameraEdge) {
                camera->y -= cameraEdge - (renderRect->y - camera->y);
            }
            if (renderRect->x - camera->x < cameraEdge) {
                camera->x -= cameraEdge - (renderRect->x - camera->x);
            }
            if (camera->y + camera->height - renderRect->y - renderRect->w < cameraEdge) {
                camera->y += cameraEdge - (camera->y + camera->height - renderRect->y - renderRect->w);
            }
            if (camera->x + camera->width - renderRect->x - renderRect->h < cameraEdge) {
                camera->x += cameraEdge - (camera->x + camera->width - renderRect->x - renderRect->h);
            }
        }

        // virtual void cameraFollow(Camera* camera) {
        //     if (!isSelected()) {
        //         return;
        //     }
        //     SDL_FRect* renderRect = getRenderPosition();
        //     if (getRenderY() - camera->y < cameraEdge) {
        //         camera->y -= cameraEdge - (getRenderY() - camera->y);
        //     }
        //     if (getRenderX() - camera->x < cameraEdge) {
        //         camera->x -= cameraEdge - (getRenderX() - camera->x);
        //     }
        //     if (camera->y + camera->height - getRenderY() - getRenderWidth() < cameraEdge) {
        //         camera->y += cameraEdge - (camera->y + camera->height - getRenderY() - getRenderWidth());
        //     }
        //     if (camera->x + camera->width - getRenderX() - getRenderHeight() < cameraEdge) {
        //         camera->x += cameraEdge - (camera->x + camera->width - getRenderX() - getRenderHeight());
        //     }
        // }

        virtual void render(State* state) {
            // cout << "Rendering footman\n";
            Camera* camera = state->camera;
            if (camera->isVisible(getRenderPosition())) {
                // cout << "Footman  x:" << location->x << ", y:" << location->y  << ", w:" << location->w  << ", h:" << location->h << "\n";
                // SDL_FRect* location = camera->translate(getRenderPosition());
                drawPosition(state);
                // renderPosition->draw(state);
                // selectPosition->draw(state);
                // outline->draw(state);
                body->render(camera->translate(getRenderPosition()));
            }
        }

        ~Footman() {
            // delete outline;
        }

};
#endif
