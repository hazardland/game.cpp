#ifndef TEST_FARM_H
#define TEST_FARM_H

using namespace std;

#include <iostream>

#include <game/sprite.h>
#include <game/unit.h>
#include <game/minimap.h>
#include <game/image.h>
#include <game/camera.h>
#include <game/event.h>

class Farm: public Unit {
    public:
    Sprite* body;
    Farm(Sprite* sprite) {
        // cout << "Creating farm\n";
        setLayer(1);
        setSize(64, 64);
        renderPosition = createPosition(0, 0, 64, 64);
        body = sprite;
    }
    virtual Uint32 getMinimapColor(SDL_PixelFormat* format) {
        return SDL_MapRGBA(format, 255, 0, 0, 255);
    }
    virtual void render(State* state) {
        Camera* camera = state->camera;
        if (camera->isVisible(getRenderPosition())) {
            drawPosition(state);
            // renderPosition->draw(state);
            body->image->render(body->getRect(), camera->translate(getRenderPosition()));
        }
    }
};
#endif
