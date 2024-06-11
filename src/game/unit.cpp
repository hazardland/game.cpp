#include "game/unit.h"

#include "game/position.h"
#include "game/map.h"
#include "game/cell.h"
#include "game/terrain.h"


// Destructor
Unit::~Unit() {
    delete renderPosition;
    delete selectPosition;
}

Unit* Unit::setMap(Map* map) {
    this->map = map;
    return this;
}

void Unit::render(State* state) {
    // Implement your rendering logic here
}

SDL_FRect* Unit::getRenderPosition() {
    return renderPosition->getSDL_FRect();
}

void Unit::addPosition(float x, float y) {
    if (canMove(x, y)) {
        Object::addPosition(x, y);
        updateMapCells();
    } else if (canMove(x, 0)) {
        Object::addPosition(x, 0);
        updateMapCells();
    } else if (canMove(0, y)) {
        Object::addPosition(0, y);
        updateMapCells();
    }
}

void Unit::setPosition(float x, float y) {
    Object::setPosition(x, y);
    updateMapCells();
}

bool Unit::isSelected() {
    return selected;
}

void Unit::select() {
    selected = true;
}

bool Unit::canMove(float dx, float dy) {
    // return true;
    // Calculate the prospective new position and dimensions
    float newX = getX() + dx;
    float newY = getY() + dy;
    float newWidth = getWidth(); // + dw;
    float newHeight = getHeight(); // + dh;

    // Check if the new position would be outside the map's bounds
    if (newX < 0 || newX + newWidth > map->tilesPerWidth * map->tileWidth ||
        newY < 0 || newY + newHeight > map->tilesPerHeight * map->tileHeight) {
        return false;
    }

    // Calculate the cells this object would occupy after moving
    int left = static_cast<int>(newX / map->tileWidth);
    int top = static_cast<int>(newY / map->tileHeight);
    int right = static_cast<int>((newX + newWidth) / map->tileWidth);
    int bottom = static_cast<int>((newY + newHeight) / map->tileHeight);

    // Correct the boundaries if they are out of the map's bounds
    left = max(0, left);
    top = max(0, top);
    right = min(map->tilesPerWidth - 1, right);
    bottom = min(map->tilesPerHeight - 1, bottom);

    // Check every cell this object would occupy after moving
    for (int i = left; i <= right; i++) {
        for (int j = top; j <= bottom; j++) {

    if (map->grid[i][j]->terrain->layer != getLayer() && getLayer() != 0) {
        return false;
    }
            // if (map->grid[i][j]->terrain->layer != getLayer() && getLayer() != 0) {
            //     return false;
            // }                    
            // Get the objects in the current cell
            auto& objects = map->grid[i][j]->objects[getLayer()];

            // Check every object in the cell
            for (const auto& object : objects) {
                // If the object is not the same as this one and they overlap, return false
                if (object != this && 
                    newX < object->getX() + object->getWidth() &&
                    newX + newWidth > object->getX() &&
                    newY < object->getY() + object->getHeight() &&
                    newY + newHeight > object->getY()) {
                    return false;
                }
            }
        }
    }

    // No collisions were found, so return true
    return true;
}

void Unit::updateMapCells() {
    if (!map) {
        return;
    }

    // Calculate the new cells this object should occupy
    int left = static_cast<int>(getX() / map->tileWidth);
    int top = static_cast<int>(getY() / map->tileHeight);
    int right = static_cast<int>((getX() + getWidth()) / map->tileWidth);
    int bottom = static_cast<int>((getY() + getHeight()) / map->tileHeight);

    // Correct the boundaries if they are out of the map's bounds
    left = max(0, left);
    top = max(0, top);
    right = min(map->tilesPerWidth - 1, right);
    bottom = min(map->tilesPerHeight - 1, bottom);

    if(left == lastCellLeft && top == lastCellTop && right == lastCellRight && bottom == lastCellBottom){
        return;
    }            
    
    // Remove object from its current cells
    for (const auto& cell : cells) {
        map->grid[cell.first][cell.second]->objects[getLayer()].remove(this);
    }
    cells.clear();


    // Add object to the new cells
    for (int i = left; i <= right; i++) {
        for (int j = top; j <= bottom; j++) {
            map->grid[i][j]->objects[getLayer()].push_back(this);
            cells.push_back(make_pair(i, j));
        }
    }     

    lastCellLeft = left;
    lastCellTop = top;
    lastCellRight = right;
    lastCellBottom = bottom;        

}

// void Unit::~Unit() {
//     // Implement destruction of the unit here if necessary
// }
