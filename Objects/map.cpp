#include "map.h"
#include <fstream>
#include <iostream>

M2DI::Map_wrapper::Map_wrapper()
{
    minimap = {0,0,sWidth/2,sHeight/2};
    image = std::make_shared<IMG_wrapper>();
    clips = std::make_shared<std::map<int,SDL_Rect>>();
}

void M2DI::Map_wrapper::render_map( SDL_wrapper & wrapper, SDL_Rect & mapPosition )
{
    screen =  {0,0,sWidth,sHeight};
    SDL_RenderSetViewport( wrapper.gRenderer, &screen );
    for( auto t : tiles )
    {
        //    std::cout << t->position.x <<"/"<< mapPosition.x << " " <<  t->position.y<<"/"<<mapPosition.y << std::endl;
        //SDL_Rect rectM = { t->position.x-mapPosition.x, t->position.y-mapPosition.y, TILESIZERENDER, TILESIZERENDER };
        //if( t->position.x<mapPosition.x-TILESIZERENDER || t->position.x>mapPosition.x+mapPosition.w || t->position.y<mapPosition.y-TILESIZERENDER || t->position.y>mapPosition.y+mapPosition.h ) continue;
        //wrapper.render_image(img, &rectM, &mapping[t->spriteType] );
        t->plot(wrapper,&mapPosition);
    }
}

void M2DI::Map_wrapper::render_minimap( SDL_wrapper & wrapper, std::vector<std::shared_ptr<object>> & objects  )
{
    //  minimap = {0,0,mWidth*sHeight/mHeight/4,sHeight/4};
    scale = (mHeight+TILESIZERENDER)/minimap.h;
    SDL_RenderSetViewport( wrapper.gRenderer, &minimap );
    for( auto t : tiles )
    {
        SDL_Rect rectMM = { (t->position.x)/scale, (t->position.y)/scale, TILESIZERENDER/scale+1, TILESIZERENDER/scale+1 };
        //   wrapper.render_image(img, &rectMM, &mapping[t->frame] );
        SDL_SetRenderDrawColor( wrapper.gRenderer, mappingColor[t->spriteType].r, mappingColor[t->spriteType].g, mappingColor[t->spriteType].b, mappingColor[t->spriteType].a );
        SDL_RenderFillRect( wrapper.gRenderer, &rectMM );
    }

    for( auto obj : objects )
    {
        SDL_Rect r =  {obj->position.x/scale,obj->position.y/scale,2,2};
        SDL_SetRenderDrawColor( wrapper.gRenderer, obj->mapColor.r, obj->mapColor.g, obj->mapColor.b, obj->mapColor.a );
        SDL_RenderFillRect( wrapper.gRenderer, &r );
    }
}

bool M2DI::Map_wrapper::load_map( std::string mapFile, int mapSizeX, int mapSizeY )
{
    mWidth = mapSizeX*TILESIZERENDER;
    mHeight= mapSizeY*TILESIZERENDER;

    int x = 0;
    int y = 0;

    std::ifstream mapStream( mapFile );
    if( mapStream.fail() ) return false;
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

            auto t = std::make_shared<object>( x, y );
            tiles.push_back(t);
            t->spriteType = tileType;
            t->image = image;
            t->clips = clips;
            t->mapColor = mappingColor[tileType];

            x += TILESIZERENDER;
            if( x >= mapSizeX*TILESIZERENDER )
            {
                x = 0;
                y += TILESIZERENDER;
            }
            //      std::cout << tiles.back()->position.x <<"/"<< tiles.back()->position.y<< std::endl;
        }
    }
    return true;
}

void M2DI::Map_wrapper::add_sprite(int type, int posX, int posY, SDL_Color col )
{
    (*clips)[type] = {posX*TILESIZE,posY*TILESIZE,TILESIZE,TILESIZE};
    mappingColor[type] = col;
}
