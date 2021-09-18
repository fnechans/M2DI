#ifndef MOVABLE_H
#define MOVABLE_H

#include "object.h"

class Movable : public Object
{
public:
    Movable(uint x=mWidth/2,uint y=mHeight/2);

    void move( std::vector<Object*> & collObjects );
    bool doesCollide( SDL_Rect & pos, std::vector<Object*> & collObjects );

    // intrinsic speed of the character
    int intrVelX;
    int intrVelY;
    direction dir;
    int speed;
    bool moved;
};

#endif // MOVABLE_H
