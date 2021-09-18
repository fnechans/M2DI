#include "object.h"

#include <fstream>

Object::Object( uint x, uint y )
{
    position.x = x;
    position.y = y;
    // DEFAULT hitbox based on tilesize, can be changed
    position.w = TILESIZE;
    position.h = TILESIZE;

    extVelX = 0;
    extVelY = 0;
    friction = TILESIZE/16;

    spriteType = 0;
    mapColor = {0,0,0,0};
    clips = std::make_shared<std::map<int,SDL_Rect>>();
}

void Object::plot(SDL_wrapper & wrapper , SDL_Rect *screen )
{
    if(screen)
    {
        if( position.x*scaleRender<screen->x-TILESIZERENDER || position.x*scaleRender>screen->x+screen->w || position.y*scaleRender<screen->y-TILESIZERENDER || position.y*scaleRender>screen->y+screen->h ) return;
        posSX = position.x*scaleRender-screen->x;
        posSY = position.y*scaleRender-screen->y;
    }

    SDL_Rect renderRect = {posSX,posSY,TILESIZERENDER,TILESIZERENDER};
    // image->set_color(255,0,0);
    wrapper.render_image(*image,&renderRect,&(*clips).at(spriteType));
}

bool Object::set_image(std::string imagePath)
{
    image =std::make_shared<IMG_wrapper>();
    return image->load_media(imagePath.c_str());
}


// non-class functions

std::vector<std::unique_ptr<Object>> import_map( std::string mapFile, int mapSizeX, int mapSizeY )
{
    int x = 0;
    int y = 0;
    std::vector<std::unique_ptr<Object>> output;

    std::ifstream mapStream( mapFile );
    if( mapStream.fail() ) return {};
    else
    {
        int tileType = -1;
        for( int i = 0; i < mapSizeX*mapSizeY; ++i )
        {
            mapStream >> tileType;
            if( mapStream.fail() )
            {
                printf( "Error loading map: Unexpected end of file!\n" );
                break;
            }

            if( tileType!=-1 )
            {
                output.push_back(std::make_unique<Object>( x, y ));
                output.back()->spriteType = tileType;
            }

            x += base::TILESIZE;
            if( x >= mapSizeX*base::TILESIZE )
            {
                x = 0;
                y += base::TILESIZE;
            }
        }
    }
    return output;
}

void Object::set_health(uint value)
{
    property["health"] = value;
    property["max_health"] = value;
}

void Object::modify_health(int value)
{
    property["health"] += value;
    if(property["health"]>property["max_health"]) property["health"] = property["max_health"];
    if(property["health"]<0) property["health"] = 0; 
}

void Object::plot_animation(SDL_wrapper & wrapper , SDL_Rect *screen, bool pause)
{
    // whether to skip the actual plotting (but keeps the animation playing and sets up the posSX/Y)
    bool skipPlot = false;
    if(screen)
    {
        if( position.x*scaleRender<screen->x-TILESIZERENDER || position.x*scaleRender>screen->x+screen->w || position.y*scaleRender<screen->y-TILESIZERENDER || position.y*scaleRender>screen->y+screen->h ) skipPlot = true;
        posSX = position.x*scaleRender-screen->x;
        posSY = position.y*scaleRender-screen->y;
    }

    for( auto iter : animations )
    {
        auto anim = iter.second;
        if( !anim->running && !anim->display ) continue;
        if(pause) anim->pause();
        SDL_Rect spriteRect = anim->get();
        if(pause) anim->play();
        if( skipPlot ) continue;
        SDL_Rect renderRect = {posSX+(int) anim->shiftX*scaleRender,posSY+(int) anim->shiftY*scaleRender,(int) spriteRect.w*scaleRenderInput,(int) spriteRect.h*scaleRenderInput};
    // image->set_color(255,0,0);
        wrapper.render_image(*anim->image,&renderRect,&spriteRect,anim->angle,anim->flip);

    }
}