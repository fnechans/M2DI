#include "player.h"

void player::evaluate(SDL_Event & e)
{
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
        case SDLK_w: velY += -speed; break;
        case SDLK_a: velX += -speed; break;
        case SDLK_s: velY +=  speed; break;
        case SDLK_d: velX +=  speed; break;
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
        case SDLK_w: velY -= -speed; break;
        case SDLK_a: velX -= -speed; break;
        case SDLK_s: velY -=  speed; break;
        case SDLK_d: velX -=  speed; break;
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
    else if(sX+sWidth>mWidth && mWidth>sWidth )
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
    else if(sY+sHeight>mHeight && mHeight > sHeight )
    {
        sY=mHeight-sHeight;
        posSY = sHeight-(mHeight-position.y);
    }
   // posSX -= TILESIZE/2;
   // posSY -= TILESIZE/2;
}
