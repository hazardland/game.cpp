#ifndef TEST_FARM_H
#define TEST_FARM_H

#include <iostream>

#include <game/sprite.h>
#include <game/unit.h>
#include <game/camera.h>
#include <game/state.h>
#include <game/image.h>

class Critter: public Unit {
    public:
    Sprite* body;
    Critter(Sprite* sprite) {
        // cout << "Creating farm\n";
        setLayer(1);
        setSize(64, 64);
        allowTerrain(2);
        renderPosition = createChildPosition(0, 0, 64, 64);
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
