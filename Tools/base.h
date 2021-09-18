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
    
    enum direction {UP,DOWN,LEFT,RIGHT};

    static const int TILESIZE = 128;
    static const int TILESIZEINPUT = 16;
    static int TILESIZERENDER;
    static double scaleRender;
    static double scaleRenderInput;

    static void set_tilerender(int);

	//Screen dimension constants
    static int sWidth;
    static int sHeight;

    //Map dimension constants
    static int mWidth;
    static int mHeight;

   // Any derived object can be in viewport, this keeps track of that
    SDL_Rect * viewPort;

};

bool key_down(SDL_Event & e, SDL_Keycode keycode);
bool key_up(SDL_Event & e, SDL_Keycode keycode);
#endif
