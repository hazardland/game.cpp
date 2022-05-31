#ifndef GAME_UTIL_H
#define GAME_UTIL_H

using namespace std;
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <game/input.h>

void replaceColors(SDL_Surface* surface, const int fromColors[][3], const int toColors[][3]) {

    int size = 0;
    while (fromColors[size][0]!=-1) {
        size++;
    }

    Uint32* from = new Uint32[size];
    Uint32* to = new Uint32[size];

    SDL_LockSurface(surface);

    // Prepare colors
    for(int i = 0; i<size; i++)
    {
        from[i] = SDL_MapRGB(surface->format, fromColors[i][0], fromColors[i][1], fromColors[i][2]);
        to[i] = SDL_MapRGB(surface->format, toColors[i][0], toColors[i][1], toColors[i][2]);
    }

    // Do the replace
    Uint8 *pixels = (Uint8 *) surface->pixels;
    int length = surface->w * surface->h;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j<size; j++) {
            if (pixels[i] == from[j]) {
                pixels[i] = to[j];
            }
        }
    }

    SDL_UnlockSurface(surface);

}

#endif
