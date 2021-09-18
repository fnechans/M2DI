#include "base.h"

base::base()
{
    viewPort = nullptr;

}

void base::set_tilerender(int TR)
{
    TILESIZERENDER=TR;
    scaleRender = ((double) TILESIZERENDER)/TILESIZE;
    scaleRenderInput = ((double) TILESIZERENDER)/TILESIZEINPUT;
}

bool key_down(SDL_Event & e, SDL_Keycode keycode)
{
    return  e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == keycode;
}

bool key_up(SDL_Event & e, SDL_Keycode keycode)
{
    return  e.type == SDL_KEYUP && e.key.repeat == 0 && e.key.keysym.sym == keycode;
}