#include "game/unit.h"

#include "game/position.h"
#include "game/map.h"
#include "game/cell.h"
#include "game/terrain.h"
#include "game/camera.h"


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
    return renderPosition->getPosition();
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

bool Unit::canMove(float deltaX, float deltaY) {
    // Compute the target position based on the movement delta
    float targetX = getX() + deltaX;
    float targetY = getY() + deltaY;
    float targetWidth = getWidth();
    float targetHeight = getHeight();
    
    // Check if the unit can legally occupy the target position
    return canOccupy(targetX, targetY, targetWidth, targetHeight);
}


bool Unit::canOccupy (float newX, float newY, float newWidth, float newHeight) {

    // std::cout << "Checking farm can exist " << newX << "," << newY << std::endl;

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
    left = std::max(0, left);
    top = std::max(0, top);
    right = std::min(map->tilesPerWidth - 1, right);
    bottom = std::min(map->tilesPerHeight - 1, bottom);

    // Check every cell this object would occupy after moving
    for (int i = left; i <= right; i++) {
        for (int j = top; j <= bottom; j++) {

            if (map->grid[i][j]->terrain->layer != getLayer() && getLayer() != 0) {
                return false;
            }
            if (!ignoresTerrain && !isTerrainAllowed(map->grid[i][j]->terrain->id)) {
                return false;
            }

            // Get the objects in the current cell
            auto& objects = map->grid[i][j]->units[getLayer()];

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
    left = std::max(0, left);
    top = std::max(0, top);
    right = std::min(map->tilesPerWidth - 1, right);
    bottom = std::min(map->tilesPerHeight - 1, bottom);

    if(left == lastCellLeft && top == lastCellTop && right == lastCellRight && bottom == lastCellBottom){
        return;
    }            
    
    // Remove object from its current cells
    for (const auto& cell : cells) {
        map->grid[cell.first][cell.second]->units[getLayer()].remove(this);
    }
    cells.clear();


    // Add object to the new cells
    for (int i = left; i <= right; i++) {
        for (int j = top; j <= bottom; j++) {
            map->grid[i][j]->units[getLayer()].push_back(this);
            cells.push_back(std::make_pair(i, j));
        }
    }     

    lastCellLeft = left;
    lastCellTop = top;
    lastCellRight = right;
    lastCellBottom = bottom;        

    
}

void Unit::drawPosition(State* state) {
    SDL_SetRenderDrawColor(state->renderer, 161, 195, 69, 255);
    SDL_RenderDrawRectF(state->renderer, state->camera->translate(getPosition()));
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 0);
}

int Unit::getLayer() {
    return layer;
}

void Unit::setLayer(int layer) {
    this->layer = layer;
}


// Allow a single terrain
void Unit::allowTerrain(int terrainId) {
    ignoresTerrain = false;
    allowedTerrains |= (1 << terrainId);
}

// Allow multiple terrains
void Unit::allowTerrains(std::initializer_list<int> terrains) {
    for (int terrainId : terrains) {
        allowTerrain(terrainId);
    }
}

// Remove a terrain
void Unit::removeTerrain(int terrainId) {
    allowedTerrains &= ~(1 << terrainId);
}

// Mark the unit as flying (ignores terrain)
void Unit::ignoreTerrain() {
    ignoresTerrain = true;
}

// Check if terrain is allowed (Flying units ignore terrain)
bool Unit::isTerrainAllowed(int terrainId) const {
    return ignoresTerrain || (allowedTerrains & (1 << terrainId));
}


bool Unit::hasMinimap() {
    return false;
}

Uint32 Unit::getMinimapColor(SDL_PixelFormat* format) {
    return 0;
}



// void Unit::~Unit() {
//     // Implement destruction of the unit here if necessary
// }
