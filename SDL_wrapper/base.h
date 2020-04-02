#ifndef BASE_H
#define BASE_H

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <memory>

class base
{
public:
    static const int TILESIZE = 32;

	//Screen dimension constants
	static const int sWidth = 640;
	static const int sHeight = 480;

    //Map dimension constants
    static const int mWidth = 640*5;
    static const int mHeight = 480*5;
};

#endif
