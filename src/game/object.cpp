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

SDL_FRect* Object::getPosition() {
    return &position;
}

Position* Object::createChildPosition(float x, float y, float width, float height) {
    Position* pos = new Position(x, y, width, height, &position.x, &position.y, &position.w, &position.h);
    childPositions.push_back(pos);
    return pos;
}


bool Object::isVisible(State* state) {
    return true;
}

void Object::update(State* state) {
    // Add implementation here
}

void Object::render(State* state) {
    // Add implementation here
}

// Notify all child positions that parent moved
void Object::updateChildPositions() {
    for (auto pos : childPositions) {
        pos->setRequiresUpdate(); // Flag the child position to recalculate
    }
}

Object::~Object() {
    // Cleanup allocated child positions
    for (auto pos : childPositions) {
        delete pos;
    }
}
