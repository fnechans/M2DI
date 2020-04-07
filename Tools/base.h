#ifndef BASE_H
#define BASE_H

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <memory>
#include <iostream>

class base
{
public:
    base();

    static const int TILESIZE = 32;
    static const int TILESIZERENDER = 64;

	//Screen dimension constants
    static int sWidth;
    static int sHeight;

    //Map dimension constants
    static int mWidth;
    static int mHeight;
};

#endif
