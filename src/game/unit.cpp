#include "game/unit.h"

#include "game/position.h"
#include "game/map.h"
#include "game/cell.h"
#include "game/terrain.h"
#include "game/camera.h"
#include "game/minimap.h"


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
    updateGrid();
}

bool Unit::isSelected() {
    return selected;
}

void Unit::select() {
    selected = true;
}

bool Unit::canMove(float deltaX, float deltaY) {
    if (map==nullptr) {
        return true;
    }

    if (deltaX==0 && deltaY==0){
        return false;
    }
    // Compute the target position based on the movement delta
    float targetX = getX() + deltaX;
    float targetY = getY() + deltaY;
    float targetWidth = getWidth();
    float targetHeight = getHeight();
    
    // Check if the unit can legally occupy the target position
    return canOccupy(targetX, targetY, targetWidth, targetHeight);
}

bool Unit::canOccupy(float newX, float newY, float newWidth, float newHeight) {

    if (map==nullptr) {
        return true;
    }

    // Fail fast if out of map bounds
    if (newX < 0 || (newX + newWidth) > map->gridWidth * map->cellWidth ||
        newY < 0 || (newY + newHeight) > map->gridHeight * map->cellHeight) {
        return false;
    }

    // Compute occupied grid cells
    int newGridFromX = std::clamp(static_cast<int>(newX / map->cellWidth), 0, map->gridWidth - 1);
    int newGridFromY = std::clamp(static_cast<int>(newY / map->cellHeight), 0, map->gridHeight - 1);
    int newGridToX = std::clamp(static_cast<int>((newX + newWidth) / map->cellWidth), 0, map->gridWidth - 1);
    int newGridToY = std::clamp(static_cast<int>((newY + newHeight) / map->cellHeight), 0, map->gridHeight - 1);

    // First check terrain (Fail fast)
    for (int i = newGridFromX; i <= newGridToX; i++) {
        for (int j = newGridFromY; j <= newGridToY; j++) {
            Cell* cell = map->grid[i][j];
            // if (!cell) continue;  // Avoid null pointer crash

            if (cell->terrain->layer != getLayer() && getLayer() != 0) {
                return false;
            }
            if (!ignoresTerrain && !isTerrainAllowed(cell->terrain->id)) {
                return false;
            }
        }
    }

    // Now check for unit collisions only if terrain check passed
    for (int i = newGridFromX; i <= newGridToX; i++) {
        for (int j = newGridFromY; j <= newGridToY; j++) {
            Cell* cell = map->grid[i][j];
            // if (!cell) continue;  // Prevent null pointer crash

            auto& objects = cell->units[getLayer()];
            for (const auto& object : objects) {
                // Skip self-check
                if (object == this) continue;

                // Now do collision check inside cell
                if (newX < object->getX() + object->getWidth() &&
                    newX + newWidth > object->getX() &&
                    newY < object->getY() + object->getHeight() &&
                    newY + newHeight > object->getY()) {
                return false;
            }
            
            }
        }
    }
    
    // No collisions
    return true;  
}

void Unit::updateGrid() {
    if (map==nullptr || !position->isReady()) {
        return;
    }

    // Calculate the new cells this object should occupy
    int newGridFromX = std::clamp(static_cast<int>(getX() / map->cellWidth), 0, map->gridWidth - 1);
    int newGridFromY = std::clamp(static_cast<int>(getY() / map->cellHeight), 0, map->gridHeight - 1);
    int newGridToX = std::clamp(static_cast<int>((getX() + getWidth()) / map->cellWidth), 0, map->gridWidth - 1);
    int newGridToY = std::clamp(static_cast<int>((getY() + getHeight()) / map->cellHeight), 0, map->gridHeight - 1);

    if (gridSet) {

        if(newGridFromX == gridFromX && newGridFromY == gridFromY && newGridToX == gridToX && newGridToY == gridToY){
            return;
        }
    
        // If old grid cells does not intersect new grid cells remove them from grid
        // Aka we moved from there
        for (int i = gridFromX; i<=gridToX; i++) {
            for (int j = gridFromY; j<=gridToY; j++) {
                // Check if i, j is outside of rectangle newGridFromX, newGridFromY, newGridToX, newGridToY
                if (i < newGridFromX || i > newGridToX || j < newGridFromY || j > newGridToY) {
                    // Remove the unit from this grid cell
                    // printf("Remove gridToX:%d, %d,%d ", gridToX, i , j);

                    map->grid[i][j]->units[getLayer()].remove(this);
                }            
            }
        }
    }

    
    // If new grid cells does not intersect new grid cells add them from grind
    for (int i = newGridFromX; i<=newGridToX; i++) {
        for (int j = newGridFromY; j<=newGridToY; j++) {
            // Check if i, j is outside of rectangle newGridFromX, newGridFromY, newGridToX, newGridToY
            if (!gridSet || i < gridFromX || i > gridToX || j < gridFromY || j > gridToY) {
                // Add new cells to grid cell
                // printf("Add %d,%d ", i , j);
                map->grid[i][j]->units[getLayer()].push_back(this);
            }            
        }
    }

    map->markObjectPositionModified();

    gridFromX = newGridFromX;
    gridFromY = newGridFromY;
    gridToX = newGridToX;
    gridToY = newGridToY;
    gridSet = true;       

    
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
