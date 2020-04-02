#include "object.h"

object::object( uint x, uint y, uint clipShift )
{
    position.x = x;
    position.y = y;
    // DEFAULT hitbox based on tilesize, can be changed
    position.w = TILESIZE;
    position.h = TILESIZE;

    speed = 2;
    frame = 0;
    mapColor = {255,0,0,255};

    for( int c = STAND; c<COUNT; ++c )
    {
    clips[c] = { c*TILESIZE, clipShift*TILESIZE, TILESIZE, TILESIZE };
    }
}

void object::plot(SDL_wrapper & wrapper , SDL_Rect *screen )
{
    if(screen)
    {
        if( position.x<screen->x || position.x>screen->x+screen->w || position.y<screen->y || position.y>screen->y+screen->h ) return;
        posSX = position.x-screen->x;
        posSY = position.y-screen->y;
        posSX -= TILESIZE/2;
        posSY -= TILESIZE/2;
    }

    image->set_color(255,0,0);
    wrapper.render_image(*image,posSX,posSY,&clips[frame/4]);

    if(moved)
    {
        ++frame;
    }

    //Cycle animation
    if( frame / 4 >= COUNT )
    {
        frame = 0;
    }
    moved = false;
}

void object::move(std::vector<std::shared_ptr<object>> collObjects )
{
    if(velX==0&&velY==0) return;

    SDL_Rect newPosition = position;
    newPosition.x += velX;
    newPosition.y += velY;

    if( newPosition.x < 0 ) newPosition.x = 0;
    if( newPosition.y < 0 ) newPosition.y = 0;
    if( newPosition.x > mWidth-newPosition.w ) newPosition.x = mWidth-newPosition.w;
    if( newPosition.y > mHeight-newPosition.h ) newPosition.y = mHeight-newPosition.h;

    for( auto obj : collObjects )
    {
        if( obj.get()==this ) continue;
        if( SDL_HasIntersection(&obj->position,&newPosition) )
        {
            // can we move just in x?
            SDL_Rect tmpPosition;
            if( velX!=0 )
            {
                tmpPosition = position;
                tmpPosition.x = newPosition.x;
                if( !SDL_HasIntersection(&obj->position,&tmpPosition) )
                {
                    newPosition = tmpPosition;
                    continue;
                }
            }
            // can we move just in y?
            if( velY!= 0)
            {
                tmpPosition = position;
                tmpPosition.y = newPosition.y;
                if( !SDL_HasIntersection(&obj->position,&tmpPosition) )
                {
                    newPosition = tmpPosition;
                    continue;
                }
            }
            return; //cannot move
        }
    }

    position = newPosition;
    moved = true;
}

bool object::set_image(std::string imagePath)
{
    image =std::make_shared<IMG_wrapper>();
    return image->load_media(imagePath.c_str());
}
