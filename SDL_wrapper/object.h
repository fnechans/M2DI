#ifndef OBJECT_H
#define OBJECT_H

#include "base.h"
#include "IMG_wrapper.h"
#include "SDL_wrapper.h"

#include<vector>

class object : public base
{
public:
    object(uint x=mWidth/2,uint y=mHeight/2,uint clipShift = 0);
    ~object(){}

    enum clipType { STAND,WALK1,WALK2,WALK3,COUNT};
    SDL_Rect clips[COUNT];
    int clipShift;

    // global coor. vars
    SDL_Rect position;
    int velX;
    int velY;

    // local coor. vard
    int posSX;
    int posSY;
    int speed;
    uint frame;
    bool moved;
    SDL_Color mapColor;

    std::shared_ptr<IMG_wrapper> image;
    void plot( SDL_wrapper & wrapper, SDL_Rect * screen = nullptr );
    void move( std::vector<std::shared_ptr<object>> collObjects );
    bool set_image(std::string imagePath);
};

#endif // OBJECT_H
