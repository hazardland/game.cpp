#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <list>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>

#include "game/object.h"
#include "game/text.h"

// class Object;
class Text;
class Minimap;
class Terrain;
class Cell;
class Image;
class Clip;
class State;

class Map : public Object {
private:
    bool debug = false;
    Image* image;
    Clip* clip;
    Text* text;
    Minimap* minimap;
    
    int random(int min, int max);
    void setTerrain(int x, int y, int type);
    int getTile(std::string key);
    std::array<int, 4> getTileBorders(int x, int y);    
    int calculateTile(int x, int y);
    void fillMap();
    
public:
    
    int cellWidth;
    int cellHeight;
    int gridWidth;
    int gridHeight;
    std::vector<std::vector<Cell*>> grid;
    std::vector<Terrain*> terrains;
    std::map<std::string, std::vector<int>> tiles;

    Map(Image* image, int cellWidth, int cellHeight, int gridWidth, int gridHeight, int layerCount=3, TTF_Font* font = NULL);
    virtual void render(State* state);
    float getWidth();
    float getHeight();
    void setMinimap(Minimap* minimap);
    void import(std::vector<std::vector<int>> data);
    void generate(int seed, float intensity, std::vector<float> ranges);
    void setDebug(bool value);
    
private:

};

#endif // GAME_MAP_H
