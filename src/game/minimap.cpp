#include <game/minimap.h>

#include "game/state.h"
#include "game/drag.h"
#include "game/cell.h"
#include "game/camera.h"
#include "game/event.h"

Minimap::Minimap(SDL_Renderer* renderer, 
        int minimapWidth,
        int minimapHeight,
        int tilesPerWidth,
        int tilesPerHeight, 
        float minimapScale
        ) {

    this->tilesPerWidth = tilesPerWidth;
    this->tilesPerHeight = tilesPerHeight;
    this->minimapScale = minimapScale;

    this->renderer = renderer;

    frame.x = 0;
    frame.y = 0;
    frame.w = minimapWidth;
    frame.h = minimapHeight;
    setPosition(0, 0);
    setSize(minimapWidth, minimapHeight);
    backgroundSurface = SDL_CreateRGBSurfaceWithFormat(0, tilesPerHeight * minimapScale, tilesPerWidth * minimapScale, 0, SDL_PIXELFORMAT_RGBA32);
    prepare();

    //foregroundSurface = SDL_CreateRGBSurfaceWithFormat(0, tilesPerHeight * scale, tilesPerWidth * scale, 0, SDL_PIXELFORMAT_RGBA32);
    foreground = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, frame.w, frame.h);
    SDL_SetTextureBlendMode(foreground, SDL_BLENDMODE_BLEND);

    drag = new Drag(getPosition(), false, true);

}

void Minimap::setTerrain(int x, int y, int red, int blue, int green) {
    SDL_Rect rect;
    rect.x = x*minimapScale;
    rect.y = y*minimapScale;
    rect.h = minimapScale<1?1:minimapScale;
    rect.w = minimapScale<1?1:minimapScale;
    SDL_FillRect(backgroundSurface, &rect, SDL_MapRGB(backgroundSurface->format, red, blue, green));
    modified = true;
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

void Minimap::prepare() {
    if (background!=NULL) {
        printf("Destroyed minimap background\n");
        SDL_DestroyTexture(background);
    }
    background = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    modified = false;
}

void Minimap::setMapData(std::vector<std::vector<Cell*>>& grid, int mapTileWidth, int mapTileHeight, float mapTileScale) {
    this->grid = &grid;
    this->mapTileWidth = mapTileWidth; 
    this->mapTileHeight = mapTileHeight; 
    this->mapTileScale = mapTileScale;
    widthRatio = (mapTileWidth*mapTileScale)/minimapScale;
    heightRatio = (mapTileHeight*mapTileScale)/minimapScale;
}

void Minimap::update(State* state) {

    Mouse* mouse = state->event->mouse;
    Camera* camera = state->camera;

    drag->update(state);

    bool manualPick = false;
    if ((mouse->leftClick || mouse->leftDragActive) && mouse->inside(getPosition())) {
        // state->camera->x = ((mouse->x - getX())/minimapScale) * (map->getWidth()/tilesPerWidth) - camera->width/2; 
        // state->camera->y = ((mouse->y - getY())/minimapScale) * (map->getHeight()/tilesPerHeight) - camera->height/2;

        camera->x = ((mouse->x - getX() + frame.x)) * widthRatio - camera->width/2; 
        camera->y = ((mouse->y - getY() + frame.y)) * heightRatio - camera->height/2;
        manualPick = true;
    }

    // Prepare scope rectangle
    scope.x = camera->x / widthRatio;
    scope.y = camera->y / heightRatio;
    scope.w = camera->width / widthRatio;
    scope.h = camera->height / heightRatio;

    if (!manualPick) {

        frame.x = scope.x + scope.w/2 - frame.w/2;
        frame.y = scope.y + scope.h/2 - frame.h/2;

        if (frame.x+frame.w > tilesPerWidth*minimapScale) {
            frame.x -= ((frame.x+frame.w) - (tilesPerWidth*minimapScale)); 
        }

        if (frame.y+frame.h > tilesPerHeight*minimapScale) {
            frame.y -= ((frame.y+frame.h) - (tilesPerHeight*minimapScale)); 
        }

        if (frame.x<0) {
            frame.x = 0;
        }
        if (frame.y<0) {
            frame.y = 0;
        }

    }
    scope.x += getX() - frame.x;
    scope.y += getY() - frame.y;

}

void Minimap::render(State* state) {
    Camera* camera = state->camera;

    if (modified) {
        prepare();
    }

    // Render backgroundSurface
    SDL_RenderCopyF(renderer, background, &frame, getPosition());

    // Lock foreground texture and prepare forground surface
    SDL_LockTextureToSurface(foreground, NULL, &foregroundSurface);
    // Clear old texture
    SDL_FillRect(foregroundSurface, NULL, 0); 

    std::map<Uint32, std::vector<SDL_Rect>> rects;

    int xTileFrom = ((frame.x*widthRatio) / (mapTileWidth*mapTileScale));
    int xTileTo = ((frame.w*widthRatio) / (mapTileWidth*mapTileScale)) + xTileFrom + 2;
    int yTileFrom = ((frame.y*heightRatio) / (mapTileHeight*mapTileScale));
    int yTileTo = ((frame.h*heightRatio) / (mapTileHeight*mapTileScale)) + yTileFrom + 2;
    if (xTileFrom<0) xTileFrom = 0;
    if (yTileFrom<0) yTileFrom = 0;
    if (xTileTo>tilesPerWidth) xTileTo = tilesPerWidth;
    if (yTileTo>tilesPerHeight) yTileTo = tilesPerHeight;

    // Iterate over cells within the frame
    for (int i = xTileFrom; i < xTileTo; i++) {
        for (int j = yTileFrom; j < yTileTo; j++) {
            // Make sure indices are in bounds
            if(i >= 0 && j >= 0 && i < grid->size() && j < (*grid)[0].size()){
                // Get cell
                Cell* cell = (*grid)[i][j];

                // Iterate over units in the cell
                for(auto& unitList : cell->units){
                    for(Unit* unit : unitList){
                        SDL_Rect rect = {
                            int(unit->getX()/widthRatio), 
                            int(unit->getY()/heightRatio),
                            int(unit->getWidth()/widthRatio),
                            int(unit->getHeight()/heightRatio)
                        };

                        rect.h = rect.h<2?2:rect.h;
                        rect.w = rect.w<2?2:rect.w;
                        if (isVisible(&rect)) {
                            translate(&rect);
                            rects[SDL_MapRGB(backgroundSurface->format,0,0,0)].push_back(rect); 
                            rects[unit->getMinimapColor(foregroundSurface->format)].push_back(rect);
                            rect.x -= 1;
                            rect.y -= 1;
                            rect.w += 2;
                            rect.h += 2;
                            rects[SDL_MapRGB(backgroundSurface->format,0,0,0)].push_back(rect); 
                        }
                    }
                }
            }
        }
    }

    // Render marked rectangles
    for (auto const& [color, items] : rects)
    {
        SDL_FillRects(foregroundSurface, items.data(), items.size(), color);
    }        
    SDL_UnlockTexture(foreground);
    SDL_RenderCopyF(renderer, foreground, NULL, getPosition());

    // Render scope rectangle        
    renderRectColor(&scope, 255, 255, 255, 70);
    renderRectBorder(&scope, 102, 5, 51, 255);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void Minimap::renderRectColor(SDL_FRect* rect, int red, int green, int blue, int alpha) {
    SDL_SetRenderDrawColor(renderer, red, blue, green, alpha);
    SDL_RenderFillRectF(renderer, rect);
}

void Minimap::renderRectBorder(SDL_FRect* rect, int red, int green, int blue, int alpha) {
    SDL_SetRenderDrawColor(renderer, red, blue, green, alpha);
    SDL_RenderDrawRectF(renderer, rect);
}

bool Minimap::isVisible(SDL_Rect* position) {
    return position->x + position->w > frame.x &&
        position->y + position->h > frame.y &&
        position->x < frame.x + frame.w &&
        position->y < frame.y + frame.h;        
    // if (position->x + position->w  > frame.x && 
    //     position->y + position->h > frame.y &&
    //     position->x < frame.x + frame.w && 
    //     position->y < frame.y + frame.h
    // ) {
    //     return true;
    // }
    // return false;

}

SDL_Rect Minimap::getMapFrame() {
    return {
            int(frame.x * widthRatio), 
            int(frame.y * heightRatio),
            int(frame.w * widthRatio),
            int(frame.h * heightRatio)
        };
}

void Minimap::translate(SDL_Rect* position) {
    position->x += - frame.x; 
    position->y += - frame.y;
}

bool Minimap::isVisible(State* state) {
    return true;
}
