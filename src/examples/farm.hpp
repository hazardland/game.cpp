#ifndef TEST_FARM_H
#define TEST_FARM_H

using namespace std;

#include <iostream>

#include <game/sprite.hpp>
#include <game/object.hpp>
#include <game/minimap.hpp>

class Farm: public Object {
    public:
    Sprite* body;
    Minimap* minimap;
    Farm(Sprite* sprite) {
        setSize(64, 64);
        body = sprite;
    }
    Farm* setMinimap(Minimap* minimap) {
        this->minimap = minimap;
        minimap->addObject(this);
        return this;
    }
    virtual SDL_Rect getMinimapRect() {
        return {
            int(getX()/minimap->widthRatio()), 
            int(getY())/minimap->heightRatio(),
            int(getWidth()/minimap->widthRatio()),
            int(getHeight()/minimap->heightRatio())
        };
    }
    virtual Uint32 getMinimapColor(SDL_PixelFormat* format) {
        return SDL_MapRGBA(format, 255, 0, 0, 255);
    }
    virtual void render(State* state) {
        Camera* camera = state->camera;
        if (camera->isVisible(getPosition())) {
            SDL_FRect* location = camera->translate(getPosition());
            body->image->render(body->getRect(), location);
        }
    }
};
#endif
