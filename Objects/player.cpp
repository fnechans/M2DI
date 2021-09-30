#include "player.h"

void Player::evaluate(SDL_Event & e)
{
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
        case SDLK_w: intrVelY += -speed; dir = UP; break;
        case SDLK_a: intrVelX += -speed; dir = LEFT; break;
        case SDLK_s: intrVelY +=  speed; dir = DOWN; break;
        case SDLK_d: intrVelX +=  speed; dir = RIGHT; break;
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
        case SDLK_w: intrVelY -= -speed; break;
        case SDLK_a: intrVelX -= -speed; break;
        case SDLK_s: intrVelY -=  speed; break;
        case SDLK_d: intrVelX -=  speed; break;
        }
    }
}

void Player::evaluate(const Uint8* state)
{
    if( state[ SDL_SCANCODE_W ] ){ intrVelX = -speed; }
    else if( state[ SDL_SCANCODE_S ] ){ intrVelY = speed;; }
    else intrVelY = 0;

    if( state[ SDL_SCANCODE_A ] ){ intrVelX = -speed; }
    else if( state[ SDL_SCANCODE_D ] ){ intrVelY = speed; }
    else intrVelX = 0;
}

void  Player::screen_position(SDL_Rect &screenRect)
{
    posSX = gameplayScreen.x+gameplayScreen.w/2;
    posSY = gameplayScreen.y+gameplayScreen.h/2;

    screenRect.x = position.x*scaleRender-posSX;
    if(screenRect.x<0)
    {
        screenRect.x=0;
        posSX =position.x*scaleRender;
    }
    else if(screenRect.x+(gameplayScreen.x+gameplayScreen.w)>mWidth*scaleRender && mWidth*scaleRender>(gameplayScreen.x+gameplayScreen.w) )
    {
        screenRect.x=mWidth*scaleRender-(gameplayScreen.x+gameplayScreen.w);
        posSX = (gameplayScreen.x+gameplayScreen.w)-(mWidth-position.x)*scaleRender;
    }

    screenRect.y = position.y*scaleRender-posSY;
    if(screenRect.y<0)
    {
        screenRect.y=0;
        posSY = position.y*scaleRender;
    }
    else if(screenRect.y+(gameplayScreen.y+gameplayScreen.h)>mHeight*scaleRender && mHeight*scaleRender > (gameplayScreen.y+gameplayScreen.h) )
    {
        screenRect.y=mHeight*scaleRender-(gameplayScreen.y+gameplayScreen.h);
        posSY = (gameplayScreen.y+gameplayScreen.h)-(mHeight-position.y)*scaleRender;
    }

    screenRect.w = gameplayScreen.w;
    screenRect.h = gameplayScreen.h;
}
