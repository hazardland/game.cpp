#ifndef GAME_MINIMAP_H
#define GAME_MINIMAP_H

#include <iostream>
#include <map>
#include <vector>

#include <SDL2/SDL_image.h>

#include "game/object.h"

class Drag;
class Cell;
class State;

class Minimap: public Object {
public:
    Minimap(SDL_Renderer* renderer, 
           int minimapWidth,
           int minimapHeight,
           int tilesPerWidth,
           int tilesPerHeight, 
           float minimapScale
    );

    void setTerrain(int x, int y, int red, int blue, int green);
    void setObject(SDL_Rect* rect, int red, int blue, int green);
    void addObject(Object* object);
    void clearObjects();
    void prepare();
    void setMapData(std::vector<std::vector<Cell*>>& grid, int mapTileWidth, int mapTileHeight, float mapTileScale);
    virtual void update(State* state) override;
    virtual void render(State* state) override;
    void renderRectColor(SDL_FRect* rect, int red, int green, int blue, int alpha=255);
    void renderRectBorder(SDL_FRect* rect, int red, int green, int blue, int alpha=255);
    virtual bool isVisible(SDL_Rect* position);
    SDL_Rect getMapFrame();
    void translate(SDL_Rect* position);
    virtual bool isVisible(State* state) override;

private:
    SDL_Renderer* renderer;
    SDL_Surface* backgroundSurface;
    SDL_Texture* background = NULL;
    SDL_Surface* foregroundSurface;
    SDL_Texture* foreground = NULL;
    SDL_FRect scope;
    SDL_Rect frame;
    std::vector<Object*> objects;
    int tilesPerWidth;
    int tilesPerHeight;
    float minimapScale;
    int scopeWidth = 2;
    SDL_Color scopeColor = {50, 205, 50};
    bool modified = false;
    Drag* drag;
    int widthRatio;
    int heightRatio;
    float mapTileScale;
    float mapTileWidth;
    float mapTileHeight;
    std::vector<std::vector<Cell*>>* grid;
};

#endif // GAME_MINIMAP_H