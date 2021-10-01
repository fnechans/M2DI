#ifndef PLAYER_H
#define PLAYER_H

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>

#include "movable.h"
#include "IMG_wrapper.h"

class Player : public Movable 
{
public:
    Player(uint x=mWidth/2,uint y=mHeight/2) : Movable(x,y) {}
    void evaluate(const Uint8* state);
    void evaluate( SDL_Event & event );
    void screen_position( SDL_Rect & screenRect );
};

#endif
