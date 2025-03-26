#ifndef WAR2_FOOTMAN_H
#define WAR2_FOOTMAN_H

#include "game/unit.h"
#include "game/animation.h"
#include "game/state.h"
#include "game/camera.h"
#include "game/text.h"

#include "examples/enum.h"

class Footman : public Unit {
private:
    bool modeSync = false;
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
    virtual bool move(int deltaTime, float directionX, float directionY);
    virtual void play(uint32_t inMode, uint32_t inModeX, uint32_t inModeY);

    // Update and rendering functions
    virtual void update(State* state) override;
    virtual void cameraFollow(Camera* camera);
    virtual void render(State* state) override;
    // virtual Uint32 getMinimapColor(SDL_PixelFormat* format);

    // Destructor
    ~Footman();
};

#endif // WAR2_FOOTMAN_H
