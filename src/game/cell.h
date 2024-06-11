#ifndef GAME_CELL_H
#define GAME_CELL_H

using namespace std;
#include <iostream>
#include <list>
#include <vector>

#include <SDL2/SDL_image.h>

class Object;
class Terrain;

class Cell {
    public:
        int tile;
        Terrain* terrain;
        SDL_Rect* rect;

        vector<list<Object*>> objects;

        Cell(int layers); // The 'objects' vector is initialized with 'layers' number of empty lists.
};

#endif
