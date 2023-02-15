#ifndef BASE_H
#define BASE_H

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include "SDL2_gfxPrimitives.h"

#include "tools.h"

typedef unsigned int uint;

namespace base
{


    const uint TILESIZEPHYSICS = 1024; // defines physical size
                                    // so it is more related to
                                    // speed and such
    const uint TILESIZEINPUT = 16;    // defines basic size of
                                    // tile in input
    uint TILESIZERENDER();              // defines size of tile on screen
                                    // so can change if zooming in/out
    double scaleRender();
    double scaleRenderInput();

    void set_tilerender(int);

    SDL_Rect toScreen(SDL_Rect* screen, const SDL_Rect& position);

    SDL_Rect fromScreen(SDL_Rect* screen, const SDL_Rect& positionScreen);
};

#endif