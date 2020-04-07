#ifndef CHARACTER_H
#define CHARACTER_H

#include "object.h"

class character : public object
{
public:
    character(uint x=mWidth/2,uint y=mHeight/2,uint clipShift = 0);
    void move( std::vector<std::shared_ptr<object>> & collObjects );
    bool doesCollide( SDL_Rect & pos, std::vector<std::shared_ptr<object>> & collObjects );
    enum clipType { STAND,WALK1,WALK2,WALK3,COUNT};
    int speed;
    int velX;
    int velY;
    bool moved;
    uint frame;
};

#endif // CHARACTER_H
