#include "movable.h"

Movable::Movable(uint x, uint y) : Object(x,y)
{
    intrVelX=0;
    intrVelY=0;

    dir = DOWN;
    speed = TILESIZEPHYSICS/16;

    moved = false;
}

void Movable::move(std::vector<Object*> & collObjects )
{
    // TODO: this needs to be done better, but not sure how right now
    reduce_to_zero(extVelX,friction);
    reduce_to_zero(extVelY,friction);
    int dX = extVelX+intrVelX;
    int dY = extVelY+intrVelY;

    moved = false;
  /*  if(intrVelX>0) dir = RIGHT;
    else if(intrVelX<0) dir = LEFT;
    else if(intrVelY>0) dir = DOWN;
    else if(intrVelY<0) dir = UP;*/
    if(dX==0&&dY==0) return;

    SDL_Rect newPosition = position;
    while( dX || dY )
    {
        newPosition = position;
        newPosition.x += dX;
        newPosition.y += dY;

        if( newPosition.x < 0 ) newPosition.x = 0;
        if( newPosition.y < 0 ) newPosition.y = 0;
        if( newPosition.x > mWidth-newPosition.w ) newPosition.x = mWidth-newPosition.w;
        if( newPosition.y > mHeight-newPosition.h ) newPosition.y = mHeight-newPosition.h;

        if( !doesCollide(newPosition, collObjects) )
        {
            position = newPosition;
            moved = true;
            return;
        }

        reduce_to_zero(dX,1);
        reduce_to_zero(dY,1);
    }

    dX = extVelX+intrVelX;
    dY = extVelY+intrVelY;

    while( dX )
    {
        newPosition = position;
        newPosition.x += dX;

        if( newPosition.x < 0 ) newPosition.x = 0;
        if( newPosition.y < 0 ) newPosition.y = 0;
        if( newPosition.x > mWidth-newPosition.w ) newPosition.x = mWidth-newPosition.w;
        if( newPosition.y > mHeight-newPosition.h ) newPosition.y = mHeight-newPosition.h;

        if( !doesCollide(newPosition, collObjects) )
        {
            position = newPosition;
            moved = true;
            return;
        }

        reduce_to_zero(dX,1);
    }

    dX = extVelX+intrVelX;
    dY = extVelY+intrVelY;

    while( dY )
    {
        newPosition = position;
        newPosition.y += dY;

        if( newPosition.x < 0 ) newPosition.x = 0;
        if( newPosition.y < 0 ) newPosition.y = 0;
        if( newPosition.x > mWidth-newPosition.w ) newPosition.x = mWidth-newPosition.w;
        if( newPosition.y > mHeight-newPosition.h ) newPosition.y = mHeight-newPosition.h;

        if( !doesCollide(newPosition, collObjects) )
        {
            position = newPosition;
            moved = true;
            return;
        }

        reduce_to_zero(dY,1);
    }
}

bool Movable::doesCollide( SDL_Rect & pos, std::vector<Object*> & collObjects )
{

    for( auto obj : collObjects )
    {
        if( obj==this ) continue;
        if( SDL_HasIntersection(&obj->position,&pos) ) return true;
    }
    return false;
}
