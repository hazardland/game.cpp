#include "game/object.h"


#include "game/state.h"
#include "game/position.h"
#include "game/scene.h"
#include "game/camera.h"


int Object::count = 0;

void Object::setId() {
    if (id==0) {
        Object::count++;
        id = Object::count;
    }
}

int Object::getId() {
    setId();
    return id;
}

void Object::addPosition(float x, float y) {
    position.x += x;
    position.y += y;
}

void Object::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void Object::setSize(float width, float height) {
    position.w = width;
    position.h = height;
}

void Object::setX(float x) {
    position.x = x;
}

void Object::setY(float y) {
    position.y = y;
}

void Object::setHeight(float height) {
    position.h = height;
}

void Object::setWidth(float width) {
    position.w = width;
}

float Object::getX() {
    return position.x;
}

float Object::getY() {
    return position.y;
}

float Object::getHeight() {
    return position.h;
}

float Object::getWidth() {
    return position.w;
}

int Object::getLayer() {
    return layer;
}

void Object::setLayer(int layer) {
    this->layer = layer;
}

SDL_FRect* Object::getPosition() {
    return &position;
}

SDL_FRect* Object::getRenderPosition() {
    return &position;
}

Position* Object::createPosition(float x, float y, float width, float height) {
    return new Position(x, y, width, height, &position.x, &position.y, &position.w, &position.h);
}

void Object::drawPosition(State* state) {
    SDL_SetRenderDrawColor(state->renderer, 161, 195, 69, 255);
    SDL_RenderDrawRectF(state->renderer, state->camera->translate(getPosition()));
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 0);
}

bool Object::isVisible(State* state) {
    return state->camera->isVisible(getRenderPosition());
}

bool Object::hasMinimap() {
    return false;
}

Uint32 Object::getMinimapColor(SDL_PixelFormat* format) {
    return 0;
}

void Object::update(State* state) {
    // Add implementation here
}

void Object::render(State* state) {
    // Add implementation here
}
