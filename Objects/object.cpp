#include "object.h"

object::object( uint x, uint y, uint clipShift )
{
    position.x = x;
    position.y = y;
    // DEFAULT hitbox based on tilesize, can be changed
    position.w = TILESIZERENDER;
    position.h = TILESIZERENDER;

    spriteType = 0;
    mapColor = {0,0,0,0};
    clips = std::make_shared<std::map<int,SDL_Rect>>();
}

void object::plot(SDL_wrapper & wrapper , SDL_Rect *screen )
{
    if(screen)
    {
        if( position.x<screen->x-TILESIZERENDER || position.x>screen->x+screen->w || position.y<screen->y-TILESIZERENDER || position.y>screen->y+screen->h ) return;
        posSX = position.x-screen->x;
        posSY = position.y-screen->y;
    }

    renderRect = {posSX,posSY,TILESIZERENDER,TILESIZERENDER};
   // image->set_color(255,0,0);
    wrapper.render_image(*image,&renderRect,&(*clips)[spriteType]);
}

bool object::set_image(std::string imagePath)
{
    image =std::make_shared<IMG_wrapper>();
    return image->load_media(imagePath.c_str());
}
