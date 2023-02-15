#include "base.h"

// "Global variables for rendering, accesible only via functions
uint gTILESIZERENDER{0};              // defines size of tile on screen
                                  // so can change if zooming in/out
double gscaleRender{1.};
double gscaleRenderInput{1.};

uint base::TILESIZERENDER(){ return gTILESIZERENDER; };              // defines size of tile on screen
                                    // so can change if zooming in/out
double base::scaleRender(){ return gscaleRender; }
double base::scaleRenderInput(){ return gscaleRenderInput; };

void base::set_tilerender(int TR)
{
    gTILESIZERENDER = TR;
    gscaleRender = ((double)gTILESIZERENDER) / TILESIZEPHYSICS;
    gscaleRenderInput = ((double)gTILESIZERENDER) / TILESIZEINPUT;
}

SDL_Rect base::toScreen(SDL_Rect *screen, const SDL_Rect &position)
{
    SDL_Rect positionScreen;
    positionScreen.x = position.x * gscaleRender - screen->x;
    positionScreen.y = position.y * gscaleRender - screen->y;
    return positionScreen;
}

SDL_Rect base::fromScreen(SDL_Rect *screen, const SDL_Rect &positionScreen)
{
    SDL_Rect position;
    position.x = (positionScreen.x + screen->x) / gscaleRender;
    position.y = (positionScreen.y + screen->y) / gscaleRender;
    return position;
}