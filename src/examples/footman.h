#ifndef WAR2_FOOTMAN_H
#define WAR2_FOOTMAN_H

#include <iostream>

#include "game/unit.h"
#include "game/animation.h"
#include "game/position.h"
#include "game/state.h"
#include "game/event.h"
#include "game/camera.h"
#include "game/clock.h"
#include "game/text.h"

#include "examples/enum.h"

class Footman : public Unit {
public:
    Animation* body;
    int maxSpeed = 100;
    int speed = 95;
    int mode = IDLE;
    int modeX = RIGHT;
    int modeY = DOWN;
    int cameraEdge = 100;
    int cameraStep = 1;
    bool cameraScroll = true;
    Text* text;

    // Constructor
    Footman(Sprite* sprite, TTF_Font* font);

    // Movement functions
    virtual void rotate(float directionX, float directionY);
    bool safeMove(float byX, float byY);
    virtual bool move(int deltaTime, float directionX, float directionY);

    // Update and rendering functions
    virtual void update(State* state) override;
    virtual void cameraFollow(Camera* camera);
    virtual void render(State* state);
    virtual Uint32 getMinimapColor(SDL_PixelFormat* format);

    // Destructor
    ~Footman();
};

#endif // WAR2_FOOTMAN_H
