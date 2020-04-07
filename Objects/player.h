#ifndef CHAR_H
#define CHAR_H

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>

#include "character.h"
#include "IMG_wrapper.h"
#include "SDL_wrapper.h"

class player : public character
{
public:
    player(uint x=mWidth/2,uint y=mHeight/2,uint clipShift = 0) : character(x,y,clipShift) {}
    void evaluate(const Uint8* state);
    void evaluate( SDL_Event & event );
    void screen_position( int & sX, int & sY );

};

#endif
