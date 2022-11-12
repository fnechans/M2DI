#ifndef BASE_H
#define BASE_H

//Using SDL and standard IO
#include <SDL2/SDL.h>
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

class base
{
public:
    base();

    static uint TICKS_PER_SECOND;
    static uint FRAMES_PER_SECOND;
    static float DELTA_T; // ms
    static bool VSYNC;

    static const uint TILESIZEPHYSICS = 1024; // defines physical size
                                            // so it is more related to
                                            // speed and such
    static const uint TILESIZEINPUT = 16;    // defines basic size of
                                            // tile in input
    static uint TILESIZERENDER;              // defines size of tile on screen
                                            // so can change if zooming in/out
    static double scaleRender;
    static double scaleRenderInput;

    static void set_tilerender(int);

    //Map dimension constants
    static uint mWidth;
    static uint mHeight;

    static SDL_Rect toScreen(SDL_Rect* screen, const SDL_Rect& position)
    {
        SDL_Rect positionScreen;
        positionScreen.x = position.x * scaleRender - screen->x;
        positionScreen.y = position.y * scaleRender - screen->y;
        return positionScreen;
    }

    static SDL_Rect fromScreen(SDL_Rect* screen, const SDL_Rect& positionScreen)
    {
        SDL_Rect position;
        position.x = (positionScreen.x + screen->x) / scaleRender;
        position.y = (positionScreen.y + screen->y) / scaleRender;
        return position;
    }
};

#endif