#include "character.h"

character::character(uint x, uint y, uint clipShift) : object(x,y,clipShift)
{
    velX=0;
    velY=0;
    frame=0;

    speed = TILESIZERENDER/16;

    for( int c = STAND; c<COUNT; ++c )
    {
        (*clips)[c] = { c*TILESIZE, clipShift*TILESIZE, TILESIZE, TILESIZE };
    }
}

void character::move(std::vector<std::shared_ptr<object>> & collObjects )
{
    moved = false;
    if(velX==0&&velY==0) return;

    SDL_Rect newPosition = position;
    newPosition.x += velX;
    newPosition.y += velY;

    if( newPosition.x < 0 ) newPosition.x = 0;
    if( newPosition.y < 0 ) newPosition.y = 0;
    if( newPosition.x > mWidth-newPosition.w ) newPosition.x = mWidth-newPosition.w;
    if( newPosition.y > mHeight-newPosition.h ) newPosition.y = mHeight-newPosition.h;

    if( doesCollide(newPosition, collObjects) )
    {
        SDL_Rect tmpPosition;
        // can we move just in x?
        if( velX!=0 )
        {
            tmpPosition = position;
            tmpPosition.x = newPosition.x;
            if( !doesCollide(tmpPosition, collObjects) )
            {
                position = tmpPosition;
                moved = true;
            }
        }
        // can we move just in y?
        if( velY!=0 )
        {
            tmpPosition = position;
            tmpPosition.y = newPosition.y;
            if( !doesCollide(tmpPosition, collObjects) )
            {
                position = tmpPosition;
                moved = true;
            }
        }
    }
    else
    {
        position = newPosition;
        moved = true;
    }
    if(moved)
    {
        ++frame;
        if( frame/4 >= clips->size() )
        {
            frame = 0;
        }
        spriteType = frame/4;
    }
}

bool character::doesCollide( SDL_Rect & pos, std::vector<std::shared_ptr<object>> & collObjects )
{

    for( auto obj : collObjects )
    {
        if( obj.get()==this ) continue;
        if( SDL_HasIntersection(&obj->position,&pos) ) return true;
    }
    return false;
}
