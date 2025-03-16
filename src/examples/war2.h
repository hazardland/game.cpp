#ifndef WAR2_SCENE_H
#define WAR2_SCENE_H

#include <vector>
#include <map>
#include <iostream>

#include "game/scene.h"
#include "game/text.h"
#include "game/fps.h"
#include "game/map.h"
#include "game/minimap.h"
#include "game/terrain.h"
#include "game/image.h"
#include "game/select.h"
#include "game/window.h"
#include "game/cooldown.h"

#include "examples/enum.h"
#include "examples/footman.h"
#include "examples/farm.h"

class Warcraft : public Scene {
    using Scene::Scene;

private:
    TTF_Font* font;
    TTF_Font* fontSmall;
    Text* fps;
    Cooldown* cooldown;

    int ticks;

public:
    // Constants
    static constexpr int WIDTH = 500;
    static constexpr int HEIGHT = 500;
    static constexpr int SPRITE_FOOTMAN_RED = 1;
    static constexpr int SPRITE_HUMAN_FARM = 2;

    static constexpr int LAYER_WATER = 0;
    static constexpr int LAYER_GROUND = 1;
    static constexpr int LAYER_AIR = 2;

    static constexpr int TERRAIN_WATER = 0;
    static constexpr int TERRAIN_SHORE = 1;
    static constexpr int TERRAIN_GROUND = 2;
    static constexpr int TERRAIN_FOREST = 3;

    // Methods
    virtual void prepare();
    void generate();
    virtual void update(State* state) override;
};

#endif // WAR2_SCENE_H
