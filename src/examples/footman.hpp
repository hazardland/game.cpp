#ifndef TEST_FOOTMAN_H
#define TEST_FOOTMAN_H

using namespace std;

#include <iostream>

#include <game/sprite.hpp>
#include <game/object.hpp>
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
class Footman: public Object {
    public:
        Animation* body;
        Rectangle* outline;
        int maxSpeed = 100;
        int speed = 99;
        int mode = STAND;
        int modeX = RIGHT;
        int modeY = DOWN;
        int cameraEdge = 100;
        int cameraStep = 1;
        bool cameraScroll = true; 
        Map* map;
        Minimap* minimap;
        Footman(Sprite* sprite) {
            // cout << "Creating footman\n";
            setSize(72, 72);
            // setPosition(x, y);
            body = new Animation(sprite, mode+modeX+modeY);
            body->pause = 0;
            outline = new Rectangle(sprite->image->renderer);
            outline->setSize(getWidth()/3, getHeight()/3);
            outline->border = SDL_Color{161, 195, 69, 255};
        }
        Footman* setMap(Map* map) {
            this->map = map;
            return this;
        }
        Footman* setMinimap(Minimap* minimap) {
            this->minimap = minimap;
            minimap->addObject(this);
            return this;
        }
        virtual Uint32 getMinimapColor(SDL_PixelFormat* format) {
            return SDL_MapRGBA(format, 255, 255, 0, 255);
        }
        virtual SDL_Rect getMinimapRect() {
            return {
                int(getX()/minimap->widthRatio()), 
                int(getY())/minimap->heightRatio(),
                int(getWidth()/minimap->widthRatio()),
                int(getHeight()/minimap->heightRatio())
            };
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
                    cameraFollow(state->camera);
                }
            }
            body->play(mode+modeX+modeY);
            body->update(state->clock->delta);
        }
        virtual void cameraFollow(Camera* camera) {
            if (getY() - camera->y < cameraEdge) {
                camera->y -= cameraEdge - (getY() - camera->y);
            }
            if (getX() - camera->x < cameraEdge) {
                camera->x -= cameraEdge - (getX() - camera->x);
            }
            if (camera->y + camera->height - getY() - getWidth() < cameraEdge) {
                camera->y += cameraEdge - (camera->y + camera->height - getY() - getWidth());
            }
            if (camera->x + camera->width - getX() - getHeight() < cameraEdge) {
                camera->x += cameraEdge - (camera->x + camera->width - getX() - getHeight());
            }
        }
        virtual void render(State* state) {
            Camera* camera = state->camera;
            if (camera->isVisible(getPosition())) {
                SDL_FRect* location = camera->translate(getPosition());
                outline->setPosition(location->x+25, location->y+30);
                outline->render(state);
                body->render(location);
            }
        }

};
#endif
