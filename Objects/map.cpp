#include "map.h"
#include <iostream>

M2DI::Map_wrapper::Map_wrapper()
{
    gameplayScreen = {0,0,sWidth-TILESIZEINPUT*6,sHeight}; //hardcoded for now
    viewPort = new SDL_Rect({sWidth-TILESIZEINPUT*6,0,TILESIZEINPUT*6,sHeight});
    image = std::make_shared<IMG_wrapper>();
    clips = std::make_shared<std::map<int,SDL_Rect>>();
}

void M2DI::Map_wrapper::render_map( SDL_wrapper & wrapper, SDL_Rect & mapPosition )
{
    gameplayScreen = {0,0,sWidth-TILESIZEINPUT*12,sHeight}; //hardcoded for now
    SDL_RenderSetViewport( wrapper.gRenderer, &gameplayScreen );
    for( auto& t : tiles )
    {
        t->plot(wrapper,&mapPosition);
    }
    for( auto& t : blocks )
    {
        t->plot(wrapper,&mapPosition);
    }
}

void M2DI::Map_wrapper::render_minimap( SDL_wrapper & wrapper, std::vector<Object*> & objects  )
{
    *viewPort = {sWidth-TILESIZEINPUT*12,0,TILESIZEINPUT*12,TILESIZEINPUT*12};
    scale = ((double) mWidth)/viewPort->w;
    SDL_RenderSetViewport( wrapper.gRenderer, viewPort );
    for( auto const& t : tiles )
    {
        SDL_Rect rectMM = { (int) (t->position.x)/scale, (int) (t->position.y)/scale, (int) TILESIZE/scale+1, (int) TILESIZE/scale+1 };
        SDL_SetRenderDrawColor( wrapper.gRenderer, mappingColor[t->spriteType].r, mappingColor[t->spriteType].g, mappingColor[t->spriteType].b, mappingColor[t->spriteType].a );
        SDL_RenderFillRect( wrapper.gRenderer, &rectMM );
    }
    for( auto const& t : blocks )
    {
        SDL_Rect rectMM = { (int) (t->position.x)/scale,(int) (t->position.y)/scale, (int) TILESIZE/scale+1, (int) TILESIZE/scale+1 };
        SDL_SetRenderDrawColor( wrapper.gRenderer, mappingColor[t->spriteType].r, mappingColor[t->spriteType].g, mappingColor[t->spriteType].b, mappingColor[t->spriteType].a );
        SDL_RenderFillRect( wrapper.gRenderer, &rectMM );
    }

    for( auto obj : objects )
    {
        SDL_Rect r =  {(int) obj->position.x/scale, (int) obj->position.y/scale, 2, 2};
        SDL_SetRenderDrawColor( wrapper.gRenderer, obj->mapColor.r, obj->mapColor.g, obj->mapColor.b, obj->mapColor.a );
        SDL_RenderFillRect( wrapper.gRenderer, &r );
    }
}

bool M2DI::Map_wrapper::load_map( std::string mapFile, int mapSizeX, int mapSizeY )
{
    mWidth = mapSizeX*TILESIZE;
    mHeight= mapSizeY*TILESIZE;

    tiles = import_map(mapFile,mapSizeX,mapSizeY);
    if(tiles.empty()) return false;
    for( auto& t : tiles )
    {
        t->mapColor = mappingColor[t->spriteType];
        t->image = image;
        t->clips = clips;
    }
    return true;
}

bool M2DI::Map_wrapper::load_blocks( std::string mapFile, int mapSizeX, int mapSizeY )
{
    blocks = import_map(mapFile,mapSizeX,mapSizeY);
    if(blocks.empty()) return false;
    for( auto& t : blocks )
    {
        t->mapColor = mappingColor[t->spriteType];
        t->image = image;
        t->clips = clips;
        t->set_health(mappingHealth[t->spriteType]);
    }
    return true;
}

void M2DI::Map_wrapper::add_sprite(int type, int posX, int posY, SDL_Color col, uint health )
{
    (*clips)[type] = {posX*TILESIZEINPUT,posY*TILESIZEINPUT,TILESIZEINPUT,TILESIZEINPUT};
    mappingColor[type] = col;
    mappingHealth[type] = health;
}
