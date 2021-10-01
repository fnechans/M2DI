#ifndef CHARACTER_H
#define CHARACTER_H

#include "movable.h"

class Character : public Movable 
{
public:
    Character(uint x=mWidth/2,uint y=mHeight/2);

    void follow_path();
    void plot_path( Window & wrapper, SDL_Rect * screen);

    bool moved;
    std::vector<Object *> path = {};
    Object * target;
};

#endif // CHARACTER_H
