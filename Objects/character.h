#ifndef CHARACTER_H
#define CHARACTER_H

#include "object.h"
#include "melee.h"

class Melee_instance;

class Character : public Object 
{
public:
    Character(uint x=mWidth/2,uint y=mHeight/2);

    void follow_path();
    void plot_path( Window & wrapper, SDL_Rect * screen);

    std::vector<Object *> path = {};
    Object * target;

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

#endif // CHARACTER_H
