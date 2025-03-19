#ifndef WAR2_FARM_H
#define WAR2_FARM_H

#include <iostream>
#include "game/sprite.h"
#include "game/unit.h"
#include "game/minimap.h"
#include "game/image.h"
#include "game/camera.h"
#include "game/input.h"
#include "game/state.h"

class Farm : public Unit {
public:
    Sprite* body;

    // Constructor
    Farm(Sprite* sprite);

    // Virtual functions
    // virtual Uint32 getMinimapColor(SDL_PixelFormat* format) override;
    virtual void render(State* state) override;
};

#endif // FARM_HPP
