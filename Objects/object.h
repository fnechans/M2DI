#ifndef OBJECT_H
#define OBJECT_H

#include "base.h"
#include "IMG_wrapper.h"
#include "SDL_wrapper.h"

#include<vector>
#include<map>

class object : public base
{
public:
    object(uint x=mWidth/2,uint y=mHeight/2,uint clipShift = 0);
    ~object(){}

    std::shared_ptr<std::map<int,SDL_Rect>> clips;
    int clipShift;

    // global coor. vars
    SDL_Rect position;
    SDL_Rect renderRect;

    // local coor. vard
    int posSX;
    int posSY;
    uint spriteType;
    SDL_Color mapColor;

    std::shared_ptr<IMG_wrapper> image;
    void plot( SDL_wrapper & wrapper, SDL_Rect * screen = nullptr );
    bool set_image(std::string imagePath);
};

#endif // OBJECT_H
