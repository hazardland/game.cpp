#include "game/object.h"


#include "game/state.h"
#include "game/position.h"
#include "game/scene.h"
#include "game/camera.h"


int Object::count = 0;

Object::Object() : position(new Position()) {}

// Object::Object(Position* position) {
//     this->position = position;
// }

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
    // position.x += x;
    // position.y += y;
    position->addPosition(x, y);
}

void Object::setPosition(float x, float y) {
    // position.x = x;
    // position.y = y;
    position->setPosition(x, y);
}

void Object::setSize(float width, float height) {
    // position.w = width;
    // position.h = height;
    position->setSize(width, height);
}

void Object::setX(float x) {
    // position.x = x;
    position->setX(x);
}

void Object::setY(float y) {
    // position.y = y;
    position->setY(y);
}

void Object::setHeight(float height) {
    // position.h = height;
    position->setHeight(height);
}

void Object::setWidth(float width) {
    // position.w = width;
    position->setWidth(width);
}

float Object::getX() {
    // return position->x;
    return position->getX();
}

float Object::getY() {
    // return position->y;
    return position->getY();
}

float Object::getHeight() {
    // return position->h;
    return position->getHeight();
}

float Object::getWidth() {
    // return position->w;
    return position->getWidth();
}

SDL_FRect* Object::getPosition() {
    return position->getSDL_FRect();
}

// This is kind of not correct but I need ch
Position* Object::createChildPosition(float x, float y, float width, float height) {
    Position* pos =  new Position(
                        x, y, 
                        width, height, 
                        &position->rect.x,
                        &position->rect.y, 
                        &position->rect.w, 
                        &position->rect.h
                    );
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