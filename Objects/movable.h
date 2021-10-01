#ifndef MOVABLE_H
#define MOVABLE_H

#include "object.h"
#include "melee.h"


// TODO: Could this be avoided?
class Melee_instance;

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
    std::map<std::string,Melee_instance> melees;
};

#endif // MOVABLE_H
