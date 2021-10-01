#include "tilemap.h"
#include <iostream>

Map_wrapper::Map_wrapper(SDL_Rect border) : base(), gameplayBorder(border)
{
    image = std::make_shared<IMG_wrapper>();
    clips = std::make_shared<std::map<int,SDL_Rect>>();
}

void Map_wrapper::render_map( Window & window, SDL_Rect & mapPosition )
{
    gameplayScreen = {gameplayBorder.x, gameplayBorder.y,window.sWidth-gameplayBorder.w,window.sHeight-gameplayBorder.h}; 

    window.viewPort(&gameplayScreen);
    for( auto& tile : tiles )
    {
        tile.plot(window,&mapPosition);
    }
    // TODO: move blocks from tilemap!
    for( auto& tile : blocks )
    {
        tile.plot(window,&mapPosition);
    }
}

void Map_wrapper::render_minimap( Window& window, std::vector<Object*> & objects  )
{
    // TODO: custom setting in future:
    minimapViewPort = {window.sWidth-gameplayBorder.w, 0, gameplayBorder.w, gameplayBorder.w};
    scale = ((double) mWidth)/minimapViewPort.w;
    window.viewPort(&minimapViewPort);
    for( auto const& tile : tiles )
    {
        SDL_Rect rectMM = { (int) (tile.position.x/scale),
                (int) (tile.position.y/scale),
                (int) (TILESIZEPHYSICS/scale)+1,
                (int) (TILESIZEPHYSICS/scale)+1
            };
        window.drawColorRect(&rectMM, mappingColor[tile.spriteType]);
    }

    for( auto const& tile : blocks )
    {
        SDL_Rect rectMM = { (int) (tile.position.x/scale),
                (int) (tile.position.y/scale),
                (int) (TILESIZEPHYSICS/scale)+1,
                (int) (TILESIZEPHYSICS/scale)+1 
            };
        window.drawColorRect(&rectMM, mappingColor[tile.spriteType]);
;
    }

    for( auto& obj : objects )
    {
        SDL_Rect r =  {(int) (obj->position.x/scale),
                       (int) (obj->position.y/scale), 2, 2};
        window.drawColorRect(&r, obj->mapColor);
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

void Map_wrapper::screen_position(SDL_Rect &screenRect, Object& obj)
{
    obj.posSX = gameplayScreen.x+gameplayScreen.w/2;
    obj.posSY = gameplayScreen.y+gameplayScreen.h/2;

    screenRect.x = obj.position.x*scaleRender-obj.posSX;
    if(screenRect.x<0)
    {
        screenRect.x=0;
        obj.posSX =obj.position.x*scaleRender;
    }
    else if(screenRect.x+(gameplayScreen.x+gameplayScreen.w)>mWidth*scaleRender && mWidth*scaleRender>(gameplayScreen.x+gameplayScreen.w) )
    {
        screenRect.x=mWidth*scaleRender-(gameplayScreen.x+gameplayScreen.w);
        obj.posSX = (gameplayScreen.x+gameplayScreen.w)-(mWidth-obj.position.x)*scaleRender;
    }

    screenRect.y = obj.position.y*scaleRender-obj.posSY;
    if(screenRect.y<0)
    {
        screenRect.y=0;
        obj.posSY = obj.position.y*scaleRender;
    }
    else if(screenRect.y+(gameplayScreen.y+gameplayScreen.h)>mHeight*scaleRender && mHeight*scaleRender > (gameplayScreen.y+gameplayScreen.h) )
    {
        screenRect.y=mHeight*scaleRender-(gameplayScreen.y+gameplayScreen.h);
        obj.posSY = (gameplayScreen.y+gameplayScreen.h)-(mHeight-obj.position.y)*scaleRender;
    }

    screenRect.w = gameplayScreen.w;
    screenRect.h = gameplayScreen.h;
}
