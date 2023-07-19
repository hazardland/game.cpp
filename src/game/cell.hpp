#ifndef GAME_CELL
#define GAME_CELL

using namespace std;
#include <iostream>
#include <list>

#include <SDL2/SDL_image.h>

#include <game/state.hpp>
#include <game/terrain.hpp>
#include <game/object.hpp>

class Cell {
    public:

        int tile;
        Terrain* terrain;
        SDL_Rect* rect;

        vector<list<Object*>> objects;

        Cell(int layers) : objects(layers) {
            // The 'objects' vector is initialized with 'layers' number of empty lists.
        }
};

#endif
