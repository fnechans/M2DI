#include "player.h"

void player::evaluate(SDL_Event & e)
{
    if( e.type == SDL_KEYDOWN )
    {
        switch( e.key.keysym.sym )
        {
        case SDLK_w:
            position.y += -speed; break;
        case SDLK_a:
            position.x += -speed; break;
        case SDLK_s:
            position.y +=  speed; break;
        case SDLK_d:
            position.x +=  speed; break;
        }
    }
}

void player::evaluate(const Uint8* state)
{
    if( state[ SDL_SCANCODE_W ] ){ velY = -speed; }
    else if( state[ SDL_SCANCODE_S ] ){ velY = speed;; }
    else velY = 0;

    if( state[ SDL_SCANCODE_A ] ){ velX = -speed; }
    else if( state[ SDL_SCANCODE_D ] ){ velX = speed; }
    else velX = 0;
}

void  player::screen_position( int & sX, int & sY )
{
    posSX = sWidth/2;
    posSY = sHeight/2;

    sX = position.x-sWidth/2;
    if(sX<0)
    {
        sX=0;
        posSX =position.x;
    }
    if(sX+sWidth>mWidth)
    {
        sX=mWidth-sWidth;
        posSX = sWidth-(mWidth-position.x);
    }

    sY = position.y-sHeight/2;
    if(sY<0)
    {
        sY=0;
        posSY = position.y;
    }
    if(sY+sHeight>mHeight)
    {
        sX=mHeight-sHeight;
        posSY = sHeight-(mHeight-position.y);
    }
    posSX -= TILESIZE/2;
    posSY -= TILESIZE/2;
}
