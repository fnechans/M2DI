#ifndef MAP_H
#define MAP_H

#include "base.h"
#include "SDL_wrapper.h"
#include "object.h"

#include <vector>

namespace M2DI {
class map : public base
{
public:
    map( SDL_Rect rect ){ location = rect; scale = 1./20.; }

    void render_map( SDL_wrapper wrapper, IMG_wrapper img, std::vector<std::shared_ptr<object>> objects  )
    {
        SDL_RenderSetViewport( wrapper.gRenderer, &location );
        wrapper.render_image(img);
/*
        for( auto obj : objects )
        {
            SDL_Rect r =  {obj->position.x*scale,obj->position.y*scale,2,2};
            SDL_SetRenderDrawColor( wrapper.gRenderer, obj->mapColor.r, obj->mapColor.g, obj->mapColor.b, obj->mapColor.a );
            SDL_RenderFillRect( wrapper.gRenderer, &r );
        }*/
    }

    SDL_Rect location;
    double scale;
};
}
#endif // MAP_H
