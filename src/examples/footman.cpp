#include "examples/footman.h"

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
        updateChildPositions();
        return true;
    } else if (byX!=0 && canMove(byX, 0)) {
        addPosition(byX, 0);
        updateGrid();
        updateChildPositions();
        return true;
    } else if (byY!=0 && canMove(0, byY)) {
        addPosition(0, byY);
        updateGrid();
        updateChildPositions();
        return true;
    }
    return false;
}

// Update function
void Footman::update(State* state) {
    Keyboard* key = state->input->keyboard;
    float directionX = 0;
    float directionY = 0;

    if (key->w) {
        directionY = -1;
    } else if (key->s) {
        directionY = 1;
    }
    if (key->a) {
        directionX = -1;
    } else if (key->d) {
        directionX = 1;
    }
    // Reduce diagonal movement speed by 40%
    if (directionX!=0 && directionY!=0) {
        directionX *= 0.6;
        directionY *= 0.6;
    }

    rotate(directionX, directionY);

    if (key->space) {
        mode = ATTACK;
    } else {
        if (directionX == 0 && directionY == 0) {
            mode = IDLE;
        } else {
            if (move(state->clock->delta, directionX, directionY)) {
                mode = MOVE;
            } else {
                mode = IDLE;
            }
            cameraFollow(state->camera);
        }
    }

    // mode = 0;
    // text->setText(std::to_string(mode));
    // text->setText(std::to_string(getX())+","+std::to_string(getY()));
    body->play(mode + modeX + modeY);

    // if (isSelected()) {
    //     text->appendText(" SEL");
    // }

    body->update(state->clock->delta);
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
