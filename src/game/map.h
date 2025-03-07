#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <list>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>

#include "game/object.h"

// class Object;
class Text;
class Minimap;
class Terrain;
class Cell;
class Image;
class Clip;
class State;

class Map : public Object {
public:
    Map(Image* image, int tileWidth, int tileHeight, int tilesPerWidth, int tilesPerHeight, float tileScale=1, int layerCount=3, Text* text = NULL);
    float getWidth();
    float getHeight();
    virtual void render(State* state);
    // virtual void update(State* state);
    void setTerrain(int x, int y, int type);
    void setMinimap(Minimap* minimap);
    int getTile(std::string key);
    void import(std::vector<std::vector<int>> data);
    void generate1(int seed, float intensity, std::vector<float> ranges);
    void generate2(int seed, float intensity, std::vector<float> ranges);
    void fillMap2();
    int getTile2(int x, int y);
    int random(int min, int max);
    std::vector<Terrain*> terrains;
    std::map<std::string, std::vector<int>> tiles;
    bool debug = false;
    std::vector<std::vector<Cell*>> grid;
    int tileWidth;
    int tileHeight;
    float tileScale;
    int tilesPerWidth;
    int tilesPerHeight;
    Image* image;
    Clip* clip;
    Text* text;
    Minimap* minimap;
    
private:

};

#endif // GAME_MAP_H
