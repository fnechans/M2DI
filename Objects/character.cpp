#include "character.h"

Character::Character(uint x, uint y) : Movable(x,y)
{
}

void Character::follow_path()
{
    reduce_to_zero(intrVelX,speed);
    reduce_to_zero(intrVelY,speed);

    if( !path.empty() )
    {
        if( fabs(position.x-path.back()->position.x)<=TILESIZE && fabs(position.y-path.back()->position.y)<=TILESIZE && path.size()>1 ) path.pop_back();
        int dirX = path.back()->position.x - position.x;
        int dirY = path.back()->position.y - position.y;

        // set speed
        if( dirX > 0 ) intrVelX = speed;
        else if( dirX < 0 ) intrVelX = -speed;
        if( dirY > 0 ) intrVelY = speed;
        else if( dirY < 0 ) intrVelY = -speed;

        // set dir
        if( fabs(dirX) > fabs(dirY) ) 
        {
            if( dirX > 0 ) dir = RIGHT;
            else if( dirX < 0 ) dir = LEFT;
        }
        else
        {
            if( dirY > 0 ) dir = DOWN;
            else if( dirY < 0 ) dir = UP;
        }
    }
}

void Character::plot_path(SDL_wrapper & wrapper , SDL_Rect *screen )
{
    if( !path.empty() )
    {
        for( auto t : path )
        {
            t->image->set_color( mapColor.r, mapColor.g, mapColor.b );
            t->plot(wrapper,screen);
            t->image->set_color( 255, 255, 255 );
        }
    }
}
