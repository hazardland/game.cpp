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
    speed = 16;
    body = new Animation(sprite, action + faceX + faceY);
    body->pause = 0;
    renderPosition = createChildPosition(-24, -24, 72, 72);
    text = new Text(font, createChildPosition(-10, -40));

    setColor ({255, 255, 0, 255});
    // setMinimapColor ({255, 255, 0, 255});
}

// Rotate the footman based on direction
void Footman::rotate(float moveX, float moveY) {
    if (moveX != 0 || moveY != 0) {
        if (moveX != 0) {
            faceX = (moveX < 0) ? LEFT : RIGHT;
        } else {
            faceX = 0;
        }
        if (moveY != 0) {
            faceY = (moveY < 0) ? UP : DOWN;
        } else {
            faceY = 0;
        }
    }
}


// Move based on deltaTime and direction if possible
bool Footman::move(int deltaTime, float moveX, float moveY) {

    float deltaX = (moveX * deltaTime) * (speed/100);
    float deltaY = (moveY * deltaTime) * (speed/100);

    
    if (deltaX!=0 && deltaY!=0 && canMove(deltaX, deltaY)) {
        addPosition(deltaX, deltaY);
        return true;
    } else if (deltaX!=0 && canMove(deltaX, 0)) {
        faceY  = 0;
        addPosition(deltaX, 0);
        return true;
    } else if (deltaY!=0 && canMove(0, deltaY)) {
        faceX  = 0;
        addPosition(0, deltaY);
        return true;
    }
    return false;
}

// Update function
void Footman::update(State* state) {

    float prevX = getX();
    float prevY = getY();
    int prevAction = action;

    Keyboard* key = state->input->keyboard;
    float moveX = 0;
    float moveY = 0;

    int isInput = false;

    if (key->w) {
        moveY = -1;
        isInput = true;
    } else if (key->s) {
        moveY = 1;
        isInput = true;
    }
    if (key->a) {
        moveX = -1;
        isInput = true;
    } else if (key->d) {
        moveX = 1;
        isInput = true;
    }
    // Reduce diagonal movement speed by 40%
    if (moveX!=0 && moveY!=0) {
        moveX *= 0.6;
        moveY *= 0.6;
    }

    rotate(moveX, moveY);

    if (key->space) {
        action = ATTACK;
        isInput = true;
    } else {
        if (moveX == 0 && moveY == 0) {
            if (!syncing) {
                action = IDLE;
            }
        } else {
            if (move(state->clock->delta, moveX, moveY)) {
                action = MOVE;
            } else {
                if (!syncing) {
                    action = IDLE;
                }
            }
            if (isSelected()) {
                cameraFollow(state->camera);
            }
        }
    }

    if (isInput) {
        syncing = false;
    }
    // action = 0;
    // text->setText(std::to_string(action));
    // text->setText(std::to_string(getX())+","+std::to_string(getY()));
    // Here it seems we need to establish some kind of relation
    // Betwee character speed and animation speed
    body->play(action + faceX + faceY, (speed/100)*6.25);

    // if (isSelected()) {
    //     text->appendText(" SEL");
    // }

    body->update(state->clock->delta);

    if (state->client!=nullptr
        && (prevX!=getX() || prevY!=getY() || prevAction!=action)
    ) {
        FootmanState msg {
            .object_id = getId(),
            .x = getX(),
            .y = getY(),
            .action = (uint32_t) action,
            .faceX = (uint32_t) faceX,
            .faceY = (uint32_t) faceY
        };
        // printf("Sending %.2f, %.2f\n", msg.x, msg.y);
        // auto encoded = Protocol::encode(msg);
        state->client->send(msg);        
    }
}

void Footman::sync(uint32_t inAction, uint32_t inFaceX, uint32_t inFaceY) {
    action = (int)inAction;
    faceX = (int)inFaceX;
    faceY = (int)inFaceY;
    syncing = true;
    // body->play((int) action);
    // syncMode = true;
}

// Camera follow function
void Footman::cameraFollow(Camera* camera) {

    float targetX = getX() + getWidth() / 2;
    float targetY = getY() + getHeight() / 2;

    int newCamX = static_cast<int>(targetX - camera->getWidth() / 2);
    int newCamY = static_cast<int>(targetY - camera->getHeight() / 2);

    camera->setX(newCamX);
    camera->setY(newCamY);
}

// Render function
void Footman::render(State* state) {
    Camera* camera = state->camera;
    if (camera->isVisible(getRenderPosition())) {
        if (isSelected()) {
            drawPosition(state);
        }
        body->render(camera->translate(getRenderPosition()));
        // text->render(state);
    }
}

// Destructor
Footman::~Footman() {
    delete body;
    delete text;
}
