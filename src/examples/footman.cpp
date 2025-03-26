#include "game/input.h"
#include "game/clock.h"
#include "game/protocol.h"

#include "examples/footman.h"
#include "examples/messages.h"

// Constructor
Footman::Footman(Sprite* sprite, TTF_Font* font) {
    setLayer(1);
    allowTerrains({1, 2});
    setSize(24, 24);
    speed = 95;
    body = new Animation(sprite, mode + modeX + modeY);
    body->pause = 0;
    renderPosition = createChildPosition(-24, -24, 72, 72);
    text = new Text(font, createChildPosition(-10, -40));
    setMinimapColor ({255, 255, 0, 255});
}

// Rotate the footman based on direction
void Footman::rotate(float directionX, float directionY) {
    if (directionX != 0 || directionY != 0) {
        if (directionX != 0) {
            modeX = (directionX < 0) ? LEFT : RIGHT;
        } else {
            modeX = 0;
        }
        if (directionY != 0) {
            modeY = (directionY < 0) ? UP : DOWN;
        } else {
            modeY = 0;
        }
    }
}


// Move based on deltaTime and direction if possible
bool Footman::move(int deltaTime, float directionX, float directionY) {

    float byX = (directionX * deltaTime) / (maxSpeed + 1 - speed);
    float byY = (directionY * deltaTime) / (maxSpeed + 1 - speed);

    if (byX!=0 && byY!=0 && canMove(byX, byY)) {
        addPosition(byX, byY);
        updateGrid();
        return true;
    } else if (byX!=0 && canMove(byX, 0)) {
        addPosition(byX, 0);
        updateGrid();
        return true;
    } else if (byY!=0 && canMove(0, byY)) {
        addPosition(0, byY);
        updateGrid();
        return true;
    }
    return false;
}

// Update function
void Footman::update(State* state) {

    float prevX = getX();
    float prevY = getY();
    int prevMode = mode;

    Keyboard* key = state->input->keyboard;
    float directionX = 0;
    float directionY = 0;

    int isInput = false;

    if (key->w) {
        directionY = -1;
        isInput = true;
    } else if (key->s) {
        directionY = 1;
        isInput = true;
    }
    if (key->a) {
        directionX = -1;
        isInput = true;
    } else if (key->d) {
        directionX = 1;
        isInput = true;
    }
    // Reduce diagonal movement speed by 40%
    if (directionX!=0 && directionY!=0) {
        directionX *= 0.6;
        directionY *= 0.6;
    }

    rotate(directionX, directionY);

    if (key->space) {
        mode = ATTACK;
        isInput = true;
    } else {
        if (directionX == 0 && directionY == 0) {
            if (!modeSync) {
                mode = IDLE;
            }
        } else {
            if (move(state->clock->delta, directionX, directionY)) {
                mode = MOVE;
            } else {
                if (!modeSync) {
                    mode = IDLE;
                }
            }
            // cameraFollow(state->camera);
        }
    }

    if (isInput) {
        modeSync = false;
    }
    // mode = 0;
    // text->setText(std::to_string(mode));
    // text->setText(std::to_string(getX())+","+std::to_string(getY()));
    body->play(mode + modeX + modeY);

    // if (isSelected()) {
    //     text->appendText(" SEL");
    // }

    body->update(state->clock->delta);

    if (state->client!=nullptr
        && (prevX!=getX() || prevY!=getY() || prevMode!=mode)
    ) {
        FootmanState msg {
            .object_id = getId(),
            .x = getX(),
            .y = getY(),
            .mode = (uint32_t) mode,
            .modeX = (uint32_t) modeX,
            .modeY = (uint32_t) modeY
        };
        // printf("Sending %.2f, %.2f\n", msg.x, msg.y);
        auto encoded = Protocol::encode(msg);
        state->client->send(encoded);        
    }
}

void Footman::play(uint32_t inMode, uint32_t inModeX, uint32_t inModeY) {
    mode = (int)inMode;
    modeX = (int)inModeX;
    modeY = (int)inModeY;
    modeSync = true;
    // body->play((int) mode);
    // syncMode = true;
}

// Camera follow function
void Footman::cameraFollow(Camera* camera) {
    if (!isSelected()) {
        return;
    }
}

// Render function
void Footman::render(State* state) {
    Camera* camera = state->camera;
    if (camera->isVisible(getRenderPosition())) {
        // drawPosition(state);
        body->render(camera->translate(getRenderPosition()));
        // text->render(state);
    }
}

// Destructor
Footman::~Footman() {
    delete body;
    delete text;
}
