#include <unordered_map>

#include <game/minimap.h>

#include "game/state.h"
#include "game/drag.h"
#include "game/cell.h"
#include "game/camera.h"
#include "game/input.h"
#include "game/screen.h"
#include "game/position.h"

Minimap::Minimap(SDL_Renderer* renderer,
        int minimapWidth,
        int minimapHeight,
        int gridWidth,
        int gridHeight,
        float minimapScale
        ) {

    this->gridWidth = gridWidth;
    this->gridHeight = gridHeight;
    this->minimapScale = minimapScale;

    this->renderer = renderer;

    frame.x = 0;
    frame.y = 0;
    frame.w = minimapWidth;
    frame.h = minimapHeight;

    position = new Position(0,0);

    setSize(minimapWidth, minimapHeight);

    backgroundSurface = SDL_CreateRGBSurfaceWithFormat(0, gridWidth * minimapScale, gridHeight * minimapScale, 0, SDL_PIXELFORMAT_RGBA32);

    resetBackgroundTexture();

    foreground = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, frame.w, frame.h);
    SDL_SetTextureBlendMode(foreground, SDL_BLENDMODE_BLEND);

    drag = new Drag(getPosition(), false, true);
    cooldown = new Cooldown(100);

}

// This is called per every tile in map
void Minimap::setTerrain(int cell, int row, int red, int blue, int green) {
    SDL_Rect rect;

    rect.x = cell*minimapScale;
    rect.y = row*minimapScale;
    rect.h = minimapScale;
    rect.w = minimapScale;

    // rect.x = x * widthRatio;
    // rect.y = y * heightRatio;
    // rect.h = widthRatio;
    // rect.w = heightRatio;


    SDL_FillRect(backgroundSurface, &rect, SDL_MapRGB(backgroundSurface->format, red, blue, green));
    mapTerrainModified = true;
}

void Minimap::setUnit(SDL_Rect* rect, int red, int blue, int green) {
    SDL_FillRect(foregroundSurface, rect, SDL_MapRGB(foregroundSurface->format, red, blue, green));
}

void Minimap::addUnit(Unit* unit) {
    units.push_back(unit);
}

void Minimap::clearUnits() {
    units.clear();
}

void Minimap::resetBackgroundTexture() {
    if (background!=NULL) {
        printf("Destroyed minimap background\n");
        SDL_DestroyTexture(background);
    }
    background = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    mapTerrainModified = false;
}

void Minimap::setMapData(std::vector<std::vector<Cell*>>& grid, int cellWidth, int cellHeight) {
    this->grid = &grid;
    this->cellWidth = cellWidth;
    this->cellHeight = cellHeight;
}

void Minimap::update(State* state) {

    Mouse* mouse = state->input->mouse;
    Camera* camera = state->camera;
    Screen* screen = state->screen;

    drag->update(state);

    // This allows to scroll map with scope drag
    bool manualPick = false;
    if ((mouse->leftClick || mouse->leftDragActive) && mouse->inside(getPosition())) {

        camera->setPosition(
            (((mouse->x - getX() + frame.x)) / minimapScale) * cellWidth - camera->getWidth()/2,
            (((mouse->y - getY() + frame.y)) / minimapScale) * cellHeight - camera->getHeight()/2
        );
        manualPick = true;
    }

    // resetBackgroundTexture scope rectangle
    scope.x = (camera->getX()/cellWidth) * minimapScale - frame.x + getX();
    scope.y = (camera->getY()/cellHeight) * minimapScale - frame.y + getY();
    scope.w = ((screen->getWidth()/cellWidth) * minimapScale) / camera->getZoom();
    scope.h = ((screen->getHeight()/cellHeight) * minimapScale) / camera->getZoom();

    if (!manualPick) {
        int oldFrameX = frame.x;
        int oldFrameY = frame.y;
        frame.x = ((camera->getX()/cellWidth) * minimapScale) - frame.w / 2;
        frame.y = ((camera->getY()/cellHeight) * minimapScale) - frame.h / 2;
        if (frame.x<0) {frame.x=0;};
        if (frame.y<0) {frame.y=0;};
        if ((frame.x+frame.w)/minimapScale>gridWidth) {frame.x=gridWidth*minimapScale-frame.w;};
        if ((frame.y+frame.h)/minimapScale>gridHeight) {frame.y=gridHeight*minimapScale-frame.h;};
        if (oldFrameX!=frame.x || oldFrameY!=frame.y) {
            objectPositionModified = true;
        }
    }

}

////////////////

// void Minimap::render(State* state) {
//     if (!visible) {
//         return;
//     }

//     SDL_RenderCopyF(renderer, background, &frame, getPosition());
//     renderRectBorder(getPosition(), 102, 5, 51, 255);

//     if (objectPositionModified && cooldown->isReady()) {


//         if (mapTerrainModified) {
//             resetBackgroundTexture();
//         }

//         SDL_LockTextureToSurface(foreground, NULL, &foregroundSurface);
//         SDL_FillRect(foregroundSurface, NULL, 0);

//         std::vector<SDL_Rect> blackBorders;
//         std::unordered_map<Uint32, std::vector<SDL_Rect>> rects;

//         // int xCellFrom = frame.x / minimapScale - 2;
//         // int xCellTo = xCellFrom + frame.w / minimapScale + 4;
//         // int yCellFrom = frame.y / minimapScale - 2;
//         // int yCellTo = frame.h / minimapScale + yCellFrom + 4;
//         // if (xCellFrom<0) xCellFrom = 0;
//         // if (yCellFrom<0) yCellFrom = 0;
//         // if (xCellTo>gridWidth) xCellTo = gridWidth;
//         // if (yCellTo>gridHeight) yCellTo = gridHeight;

//         const int xCellFrom = std::max(0, int(frame.x / minimapScale) - 2);
//         const int xCellTo = std::min(gridWidth, int(xCellFrom + frame.w / minimapScale) + 4);
//         const int yCellFrom = std::max(0, int(frame.y / minimapScale) - 2);
//         const int yCellTo = std::min(gridHeight, int(yCellFrom + frame.h / minimapScale) + 4);

//         const float widthFactor = minimapScale / cellWidth;
//         const float heightFactor = minimapScale / cellHeight;

//         // Iterate over cells within the frame
//         for (int i = xCellFrom; i < xCellTo; i++) {
//             for (int j = yCellFrom; j < yCellTo; j++) {
//                 // Make sure indices are in bounds
//                 // if(i >= 0 && j >= 0 && i < grid->size() && j < (*grid)[0].size()){
//                     // Get cell
//                     Cell* cell = (*grid)[i][j];

//                     // Iterate over units in the cell
//                     for(auto& unitList : cell->units){
//                         for(Unit* unit : unitList){
//                             // Original rectangle
//                             SDL_Rect rect = {
//                                 // int((unit->getX() / cellWidth) * minimapScale),
//                                 // int((unit->getY() / cellHeight) * minimapScale),
//                                 // int((unit->getWidth() / cellWidth) * minimapScale),
//                                 // int((unit->getHeight() / cellHeight) * minimapScale)
//                                 int(unit->getX() * widthFactor),
//                                 int(unit->getY() * heightFactor),
//                                 std::max(2, int(unit->getWidth() * widthFactor)),
//                                 std::max(2, int(unit->getHeight() * heightFactor))
//                             };

//                             // Black rectangle
//                             // rect.h = rect.h<2?2:rect.h;
//                             // rect.w = rect.w<2?2:rect.w;
//                             //if (isVisible(&rect)) {
//                                 translate(&rect);
//                                 // rects[SDL_MapRGB(backgroundSurface->format,0,0,0)].push_back(rect);
//                                 // Foreground
//                                 rects[unit->getMinimapColor(foregroundSurface->format)].push_back(rect);

//                                 // Border
//                                 rect.x -= 1;
//                                 rect.y -= 1;
//                                 rect.w += 2;
//                                 rect.h += 2;
//                                 blackBorders.push_back(rect);
//                             //}
//                         }
//                     }
//                 // }
//             }
//         }

//         // First render the black border effect
//         Uint32 blackColor = SDL_MapRGB(foregroundSurface->format, 0, 0, 0);
//         SDL_FillRects(foregroundSurface, blackBorders.data(), blackBorders.size(), blackColor);

//         // Then render unit colors
//         for (auto& [color, items] : rects) {
//             SDL_FillRects(foregroundSurface, items.data(), items.size(), color);
//         }
//         SDL_UnlockTexture(foreground);
//         cooldown->reset();
//         objectPositionModified = false;
//     }

//     SDL_RenderCopyF(renderer, foreground, NULL, getPosition());

//     // Render scope rectangle
//     renderRectColor(&scope, 255, 255, 255, 70);
//     renderRectBorder(&scope, 102, 5, 51, 255);

//     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
// }


///////////////

void Minimap::render(State* state) {
    if (!visible) {
        return;
    }

    SDL_RenderCopyF(renderer, background, &frame, getPosition());
    renderRectBorder(getPosition(), 102, 5, 51, 255);

    if (objectPositionModified && cooldown->isReady()) {


        if (mapTerrainModified) {
            resetBackgroundTexture();
        }

        SDL_LockTextureToSurface(foreground, NULL, &foregroundSurface);
        SDL_FillRect(foregroundSurface, NULL, 0);

        std::vector<SDL_Rect> blackBorders;
        std::unordered_map<Uint32, std::vector<SDL_Rect>> rects;

        const int xCellFrom = std::max(0, int(frame.x / minimapScale) - 2);
        const int xCellTo = std::min(gridWidth, int(xCellFrom + frame.w / minimapScale) + 4);
        const int yCellFrom = std::max(0, int(frame.y / minimapScale) - 2);
        const int yCellTo = std::min(gridHeight, int(yCellFrom + frame.h / minimapScale) + 4);

        const float widthFactor = minimapScale / cellWidth;
        const float heightFactor = minimapScale / cellHeight;

        // Iterate over cells within the frame
        for (int i = xCellFrom; i < xCellTo; i++) {
            for (int j = yCellFrom; j < yCellTo; j++) {

                Cell* cell = (*grid)[i][j];

                for(auto& unitList : cell->units){
                    for(Unit* unit : unitList){

                        // Original rectangle
                        SDL_Rect rect = {
                            int(unit->getX() * widthFactor) - frame.x,
                            int(unit->getY() * heightFactor) - frame.y,
                            std::max(2, int(unit->getWidth() * widthFactor)),
                            std::max(2, int(unit->getHeight() * heightFactor))
                        };

                        // Original rectangle
                        rects[unit->getMinimapColor(foregroundSurface->format)].push_back(rect);

                        // Black border rectangle
                        rect.x -= 1;
                        rect.y -= 1;
                        rect.w += 2;
                        rect.h += 2;
                        blackBorders.push_back(rect);
                    }
                }
            }
        }

        // First render the black border effect
        Uint32 blackColor = SDL_MapRGB(foregroundSurface->format, 0, 0, 0);
        SDL_FillRects(foregroundSurface, blackBorders.data(), blackBorders.size(), blackColor);

        // Then render unit colors
        for (auto& [color, items] : rects) {
            SDL_FillRects(foregroundSurface, items.data(), items.size(), color);
        }
        SDL_UnlockTexture(foreground);
        cooldown->reset();
        objectPositionModified = false;
    }

    SDL_RenderCopyF(renderer, foreground, NULL, getPosition());

    // Render scope rectangle
    renderRectColor(&scope, 255, 255, 255, 70);
    renderRectBorder(&scope, 102, 5, 51, 255);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

// void Minimap::render(State* state) {
//     if (!visible) return;

//     SDL_RenderCopyF(renderer, background, &frame, getPosition());
//     renderRectBorder(getPosition(), 102, 5, 51, 255);

//     if (objectPositionModified || cooldown->isReady()) {
//         if (mapTerrainModified) {
//             resetBackgroundTexture();
//         }

//         SDL_LockTextureToSurface(foreground, NULL, &foregroundSurface);
//         SDL_FillRect(foregroundSurface, NULL, 0);

//         std::unordered_map<Uint32, std::vector<SDL_Rect>> rects;
//         std::vector<SDL_Rect> blackBorders;  // 🔥 Separate list for black border rectangles

//         const int xCellFrom = std::max(0, int(frame.x / minimapScale) - 2);
//         const int xCellTo = std::min(gridWidth, int(xCellFrom + frame.w / minimapScale) + 4);
//         const int yCellFrom = std::max(0, int(frame.y / minimapScale) - 2);
//         const int yCellTo = std::min(gridHeight, int(yCellFrom + frame.h / minimapScale) + 4);

//         const float invCellWidth = minimapScale / cellWidth;
//         const float invCellHeight = minimapScale / cellHeight;

//         for (int i = xCellFrom; i < xCellTo; i++) {
//             for (int j = yCellFrom; j < yCellTo; j++) {
//                 Cell* cell = (*grid)[i][j];

//                 for (auto& unitList : cell->units) {
//                     for (Unit* unit : unitList) {
//                         SDL_Rect rect = {
//                             int((unit->getX() * invCellWidth) - frame.x),
//                             int((unit->getY() * invCellHeight) - frame.y),
//                             std::max(2, int(unit->getWidth() * invCellWidth)),
//                             std::max(2, int(unit->getHeight() * invCellHeight))
//                         };

//                         Uint32 color = unit->getMinimapColor(foregroundSurface->format);
//                         translate(&rect);
//                         rects[color].push_back(rect);

//                         // 🔥 Add black border effect
//                         SDL_Rect borderRect = rect;
//                         borderRect.x -= 1;
//                         borderRect.y -= 1;
//                         borderRect.w += 2;
//                         borderRect.h += 2;
//                         blackBorders.push_back(borderRect);
//                     }
//                 }
//             }
//         }

//         // 🔥 First render the black border effect
//         Uint32 blackColor = SDL_MapRGB(foregroundSurface->format, 0, 0, 0);
//         SDL_FillRects(foregroundSurface, blackBorders.data(), blackBorders.size(), blackColor);

//         // 🔥 Then render unit colors
//         for (auto& [color, items] : rects) {
//             SDL_FillRects(foregroundSurface, items.data(), items.size(), color);
//         }

//         SDL_UnlockTexture(foreground);
//         cooldown->reset();
//         objectPositionModified = false;
//     }

//     SDL_RenderCopyF(renderer, foreground, NULL, getPosition());

//     renderRectColor(&scope, 255, 255, 255, 70);
//     renderRectBorder(&scope, 102, 5, 51, 255);
// }

void Minimap::renderRectColor(SDL_FRect* rect, int red, int green, int blue, int alpha) {
    SDL_SetRenderDrawColor(renderer, red, blue, green, alpha);
    SDL_RenderFillRectF(renderer, rect);
}

void Minimap::renderRectBorder(SDL_FRect* rect, int red, int green, int blue, int alpha) {
    SDL_SetRenderDrawColor(renderer, red, blue, green, alpha);
    SDL_RenderDrawRectF(renderer, rect);
}

void Minimap::renderRectBorder(SDL_Rect* rect, int red, int green, int blue, int alpha) {
    SDL_SetRenderDrawColor(renderer, red, blue, green, alpha);
    SDL_RenderDrawRect(renderer, rect);
}

// bool Minimap::isVisible(SDL_Rect* position) {
//     return position->x + position->w > frame.x &&
//         position->y + position->h > frame.y &&
//         position->x < frame.x + frame.w &&
//         position->y < frame.y + frame.h;
//     // if (position->x + position->w  > frame.x &&
//     //     position->y + position->h > frame.y &&
//     //     position->x < frame.x + frame.w &&
//     //     position->y < frame.y + frame.h
//     // ) {
//     //     return true;
//     // }
//     // return false;

// }

// SDL_Rect Minimap::getMapFrame() {
//     return {
//             frame.x * widthRatio,
//             frame.y * heightRatio,
//             frame.w * widthRatio,
//             frame.h * heightRatio
//         };
// }

void Minimap::translate(SDL_Rect* position) {
    position->x += - frame.x;
    position->y += - frame.y;
}

bool Minimap::isVisible(State* state) {
    return visible;
}

void Minimap::toggleVisible() {
    visible = !visible;
}
void Minimap::markObjectPositionModified() {
    objectPositionModified = true;
}
