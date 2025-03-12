#include "examples/footman.h"

// Constructor
Footman::Footman(Sprite* sprite, TTF_Font* font) {
    setLayer(1);
    allowTerrains({1, 2});
    setSize(24, 24);
    body = new Animation(sprite, mode + modeX + modeY);
    body->pause = 0;
    renderPosition = createChildPosition(-24, -24, 72, 72);
    text = new Text(font, createChildPosition(-10, -40));
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

// Safe movement function
bool Footman::safeMove(float byX, float byY) {
    if (canMove(byX, byY)) {
        addPosition(byX, byY);
        updateMapCells();
        updateChildPositions();
        return true;
    } else if (canMove(byX, 0)) {
        addPosition(byX, 0);
        updateMapCells();
        updateChildPositions();
        return true;
    } else if (canMove(0, byY)) {
        addPosition(0, byY);
        updateMapCells();
        updateChildPositions();
        return true;
    }
    return false;
}

// Move based on deltaTime and direction
bool Footman::move(int deltaTime, float directionX, float directionY) {
    return safeMove((directionX * deltaTime) / (maxSpeed + 1 - speed),
                    (directionY * deltaTime) / (maxSpeed + 1 - speed));
}

// Update function
void Footman::update(State* state) {
    Keyboard* key = state->event->keyboard;
    float directionX = 0;
    float directionY = 0;

    if (key->up) {
        directionY = -1;
    } else if (key->down) {
        directionY = 1;
    }
    if (key->left) {
        directionX = -1;
    } else if (key->right) {
        directionX = 1;
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

    text->setText(std::to_string(mode + modeX + modeY));
    body->play(mode + modeX + modeY);

    if (isSelected()) {
        text->appendText(" SEL");
    }

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
        drawPosition(state);
        body->render(camera->translate(getRenderPosition()));
        text->render(state);
    }
}

// Get minimap color
Uint32 Footman::getMinimapColor(SDL_PixelFormat* format) {
    return SDL_MapRGBA(format, 255, 255, 0, 255);
}

// Destructor
Footman::~Footman() {
    delete body;
    delete text;
}
