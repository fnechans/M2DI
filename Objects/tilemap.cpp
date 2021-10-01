#include "tilemap.h"
#include <iostream>

Map_wrapper::Map_wrapper(SDL_Rect border) : gameplay(), gameplayBorder(border)
{
    gameplayScreen = {gameplayBorder.x, gameplayBorder.y,sWidth-gameplayBorder.w,sHeight-gameplayBorder.h}; 
    // TODO: custom setting in future:
  //  minimapBorder = {sWidth-gameplayBorder.w, 0, gameplayBorder.w, gameplayBorder.w};
    viewPort = SDL_Rect({sWidth-TILESIZEINPUT*6,0,TILESIZEINPUT*6,sHeight});
    image = std::make_shared<IMG_wrapper>();
    clips = std::make_shared<std::map<int,SDL_Rect>>();
}

void Map_wrapper::render_map( SDL_wrapper & wrapper, SDL_Rect & mapPosition )
{
    gameplayScreen = {gameplayBorder.x, gameplayBorder.y,sWidth-gameplayBorder.w,sHeight-gameplayBorder.h}; 
    SDL_RenderSetViewport( wrapper.gRenderer, &gameplayScreen );
    for( auto& tile : tiles )
    {
        tile.plot(wrapper,&mapPosition);
    }
    // TODO: move blocks from tilemap!
    for( auto& tile : blocks )
    {
        tile.plot(wrapper,&mapPosition);
    }
}

void Map_wrapper::render_minimap( SDL_wrapper & wrapper, std::vector<Object*> & objects  )
{
    // TODO: custom setting in future:
    viewPort = {sWidth-gameplayBorder.w, 0, gameplayBorder.w, gameplayBorder.w};
    scale = ((double) mWidth)/viewPort.w;
    SDL_RenderSetViewport( wrapper.gRenderer, &viewPort );
    for( auto const& tile : tiles )
    {
        SDL_Rect rectMM = { (int) (tile.position.x/scale),
                (int) (tile.position.y/scale),
                (int) (TILESIZEPHYSICS/scale)+1,
                (int) (TILESIZEPHYSICS/scale)+1
            };
        SDL_SetRenderDrawColor( wrapper.gRenderer, mappingColor[tile.spriteType].r, mappingColor[tile.spriteType].g, mappingColor[tile.spriteType].b, mappingColor[tile.spriteType].a );
        SDL_RenderFillRect( wrapper.gRenderer, &rectMM );
    }

    for( auto const& tile : blocks )
    {
        SDL_Rect rectMM = { (int) (tile.position.x/scale),
                (int) (tile.position.y/scale),
                (int) (TILESIZEPHYSICS/scale)+1,
                (int) (TILESIZEPHYSICS/scale)+1 
            };
        SDL_SetRenderDrawColor( wrapper.gRenderer, mappingColor[tile.spriteType].r, mappingColor[tile.spriteType].g, mappingColor[tile.spriteType].b, mappingColor[tile.spriteType].a );
        SDL_RenderFillRect( wrapper.gRenderer, &rectMM );
    }

    for( auto& obj : objects )
    {
        SDL_Rect r =  {(int) (obj->position.x/scale),
                       (int) (obj->position.y/scale), 2, 2};
        SDL_SetRenderDrawColor( wrapper.gRenderer, obj->mapColor.r, obj->mapColor.g, obj->mapColor.b, obj->mapColor.a );
        SDL_RenderFillRect( wrapper.gRenderer, &r );
    }
}

bool Map_wrapper::load_map( std::string mapFile, int mapSizeX, int mapSizeY)
{
    mWidth = mapSizeX*TILESIZEPHYSICS;
    mHeight= mapSizeY*TILESIZEPHYSICS;

    tiles = import_map(mapFile,mapSizeX,mapSizeY);
    if(tiles.empty()) return false;
    for( auto& tile : tiles )
    {
        tile.mapColor = mappingColor[tile.spriteType];
        tile.image = image;
        tile.clips = clips;
    }
    return true;
}

bool Map_wrapper::load_blocks( std::string mapFile, int mapSizeX, int mapSizeY )
{
    blocks = import_map(mapFile,mapSizeX,mapSizeY);
    if(blocks.empty()) return false;
    for( auto& tile : blocks )
    {
        tile.mapColor = mappingColor[tile.spriteType];
        tile.image = image;
        tile.clips = clips;
        if(mappingHealth[tile.spriteType]!=0)
            tile.set_health(mappingHealth[tile.spriteType]);
    }
    return true;
}

void Map_wrapper::add_sprite(int type, int posX, int posY, SDL_Color col, uint health )
{
    (*clips)[type] = {posX*TILESIZEINPUT,posY*TILESIZEINPUT,TILESIZEINPUT,TILESIZEINPUT};
    mappingColor[type] = col;
    mappingHealth[type] = health;
}
