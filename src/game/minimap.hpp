#ifndef GAME_MINIMAP
#define GAME_MINIMAP

using namespace std;

#include <iostream>
#include <map>
#include <vector>

#include <SDL2/SDL_image.h>


#include <game/object.hpp>
#include <game/object.hpp>
#include <game/state.hpp>
#include <game/drag.hpp>
#include <game/cell.hpp>

class Minimap: public Object {

    public:

    SDL_Renderer* renderer;
    SDL_Surface* backgroundSurface;
    SDL_Texture* background = NULL;
    SDL_Surface* foregroundSurface;
    SDL_Texture* foreground = NULL;
    SDL_FRect scope;
    SDL_Rect frame;

    vector<Object*> objects;

    int tilesPerWidth;
    int tilesPerHeight;
    float minimapScale;

    int scopeWidth = 2;
    SDL_Color scopeColor = {50, 205, 50};

    bool modified = false;

    Drag* drag;    

    /**
     * @brief Mini map for backgroundSurface, object markers with camera scope 
     * @param renderer Renderer used for drawing 
     * @param width Minimap view width 
     * @param height Minimap view height
     * @param tilesPerWidth Map tile count in width
     * @param tilesPerHeight Map tile count in height
     * @param scale Tile scale on minimap
     * @param map Parent map
    */
    Minimap(SDL_Renderer* renderer, 
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

    /**
     * 
    */
    void setTerrain(int x, int y, int red, int blue, int green) {
        SDL_Rect rect;
        rect.x = x*minimapScale;
        rect.y = y*minimapScale;
        rect.h = minimapScale<1?1:minimapScale;
        rect.w = minimapScale<1?1:minimapScale;
        SDL_FillRect(backgroundSurface, &rect, SDL_MapRGB(backgroundSurface->format, red, blue, green));
        modified = true;
    }

    void setObject(SDL_Rect* rect, int red, int blue, int green) {
        SDL_FillRect(foregroundSurface, rect, SDL_MapRGB(foregroundSurface->format, red, blue, green));
    }

    void addObject(Object* object) {
        objects.push_back(object);
    }

    void clearObjects() {
        objects.clear();
    }

    void prepare() {
        if (background!=NULL) {
            printf("Destroyed minimap background\n");
            SDL_DestroyTexture(background);
        }
        background = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        modified = false;
    }

    int widthRatio;
    int heightRatio;
    float mapTileScale;
    float mapTileWidth;
    float mapTileHeight;
    vector<vector<Cell*>>* grid;

    void setMapData(vector<vector<Cell*>>& grid, int mapTileWidth, int mapTileHeight, float mapTileScale) {
        this->grid = &grid;
        this->mapTileWidth = mapTileWidth; 
        this->mapTileHeight = mapTileHeight; 
        this->mapTileScale = mapTileScale;
        widthRatio = (mapTileWidth*mapTileScale)/minimapScale;
        heightRatio = (mapTileHeight*mapTileScale)/minimapScale;
    }

    virtual void update(State* state) {

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

    // virtual void render(State* state) {
    //     Camera* camera = state->camera;

    //     if (modified) {
    //         prepare();
    //     }

    //     // Render backgroundSurface
    //     SDL_RenderCopyF(renderer, background, &frame, getPosition());

    //     // Lock foreground texture and prepare forground surface
    //     SDL_LockTextureToSurface(foreground, NULL, &foregroundSurface);
    //     // Clear old texture
    //     SDL_FillRect(foregroundSurface, NULL, 0); 

    //     std::map<Uint32, vector<SDL_Rect>> rects;


    //     int xTileFrom = ((frame.x*widthRatio) / (mapTileWidth*mapTileScale));
    //     int xTileTo = ((frame.w*widthRatio) / (mapTileWidth*mapTileScale)) + xTileFrom + 2;
    //     int yTileFrom = ((frame.y*heightRatio) / (mapTileHeight*mapTileScale));
    //     int yTileTo = ((frame.h*heightRatio) / (mapTileHeight*mapTileScale)) + yTileFrom + 2;
    //     if (xTileFrom<0) xTileFrom = 0;
    //     if (yTileFrom<0) yTileFrom = 0;
    //     if (xTileTo>tilesPerWidth) xTileTo = tilesPerWidth;
    //     if (yTileTo>tilesPerHeight) yTileTo = tilesPerHeight;


    //     for (Object* object: objects) {
    //         // SDL_Rect rect = object->getMinimapRect();
    //         SDL_Rect rect = {
    //             int(object->getX()/widthRatio), 
    //             int(object->getY()/heightRatio),
    //             int(object->getWidth()/widthRatio),
    //             int(object->getHeight()/heightRatio)
    //         };

    //         rect.h = rect.h<2?2:rect.h;
    //         rect.w = rect.w<2?2:rect.w;
    //         if (isVisible(&rect)) {
    //             translate(&rect);
    //             rects[SDL_MapRGB(backgroundSurface->format,0,0,0)].push_back(rect); 
    //             rects[object->getMinimapColor(foregroundSurface->format)].push_back(rect);
    //             rect.x -= 1;
    //             rect.y -= 1;
    //             rect.w += 2;
    //             rect.h += 2;
    //             rects[SDL_MapRGB(backgroundSurface->format,0,0,0)].push_back(rect); 
    //         }
    //     }

    //     // Render marked rectangles
    //     for (auto const& [color, items] : rects)
    //     {
    //         SDL_FillRects(foregroundSurface, items.data(), items.size(), color);
    //     }        
    //     SDL_UnlockTexture(foreground);
    //     SDL_RenderCopyF(renderer, foreground, NULL, getPosition());

    //     // Render scope rectangle        
    //     renderRectColor(&scope, 255, 255, 255, 70);
    //     renderRectBorder(&scope, 102, 5, 51, 255);

    //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    // }

    virtual void render(State* state) {
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

        std::map<Uint32, vector<SDL_Rect>> rects;

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

                    // Iterate over objects in the cell
                    for(auto& objectList : cell->objects){
                        for(Object* object : objectList){
                            SDL_Rect rect = {
                                int(object->getX()/widthRatio), 
                                int(object->getY()/heightRatio),
                                int(object->getWidth()/widthRatio),
                                int(object->getHeight()/heightRatio)
                            };

                            rect.h = rect.h<2?2:rect.h;
                            rect.w = rect.w<2?2:rect.w;
                            if (isVisible(&rect)) {
                                translate(&rect);
                                rects[SDL_MapRGB(backgroundSurface->format,0,0,0)].push_back(rect); 
                                rects[object->getMinimapColor(foregroundSurface->format)].push_back(rect);
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

    void renderRectColor(SDL_FRect* rect, int red, int green, int blue, int alpha=255) {
        SDL_SetRenderDrawColor(renderer, red, blue, green, alpha);
        SDL_RenderFillRectF(renderer, rect);
    }

    void renderRectBorder(SDL_FRect* rect, int red, int green, int blue, int alpha=255) {
        SDL_SetRenderDrawColor(renderer, red, blue, green, alpha);
        SDL_RenderDrawRectF(renderer, rect);
    }

    bool isVisible(SDL_Rect* position) {
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

    SDL_Rect getMapFrame() {
        return {
                int(frame.x * widthRatio), 
                int(frame.y * heightRatio),
                int(frame.w * widthRatio),
                int(frame.h * heightRatio)
            };
    }
    
    void translate(SDL_Rect* position) {
        position->x += - frame.x; 
        position->y += - frame.y;
    }

    bool isVisible(State* state) override {
        return true;
    }


};


#endif
