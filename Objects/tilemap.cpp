#include "tilemap.h"
#include <iostream>
#include <fstream>

Map_wrapper::Map_wrapper(SDL_Rect border)
    : base(), gameplayBorder(border)
{
    image = std::make_shared<IMG_wrapper>();
}

void Map_wrapper::init(uint xMax, uint yMax)
{
    for(uint x = 0; x < xMax; x++)
    {
        for(uint y = 0; y < yMax; y++)
        {
            std::string type = std::string(1, (char)'a'+x) + std::string(1, (char)'a'+y);
            mappingClips[type] = {x * (int) TILESIZEINPUT, y * (int) TILESIZEINPUT, (int) TILESIZEINPUT, (int) TILESIZEINPUT};
            mappingColor[type] = {0,0,0,0};
            mappingHealth[type] = 0;
        }
    }
}

void Map_wrapper::render_map(Window &window, SDL_Rect &mapPosition)
{
    for (auto &tile : tiles)
    {
        tile.plot(window, &mapPosition);
    }
    // TODO: move blocks from tilemap!
    for (auto &tile : blocks)
    {
        tile.plot(window, &mapPosition);
    }
}

SDL_Rect get_tile_draw_rect(Object const &obj, double& scale)
{
    return {(int)(obj.hitbox.x / scale),
            (int)(obj.hitbox.y / scale),
            (int)(base::TILESIZEPHYSICS / scale),
            (int)(base::TILESIZEPHYSICS / scale)};
}

void Map_wrapper::render_minimap(Window &window, std::vector<Object *> &objects)
{
    // TODO: custom setting in future:
    minimapViewPort = {window.sWidth - gameplayBorder.w, 0, gameplayBorder.w, gameplayBorder.w};
    scale = ((double)mWidth) / minimapViewPort.w;
    window.viewPort(&minimapViewPort);
    for (auto &tile : tiles)
    {
        SDL_Rect rectMM = get_tile_draw_rect(tile, scale);
        window.drawColorRect(&rectMM, tile.mapColor);
    }

    for (auto &tile : blocks)
    {
        SDL_Rect rectMM = get_tile_draw_rect(tile, scale);
        window.drawColorRect(&rectMM, tile.mapColor);
        ;
    }

    for (auto &obj : objects)
    {
        SDL_Rect r = {(int)(obj->hitbox.x / scale),
                      (int)(obj->hitbox.y / scale), 2, 2};
        window.drawColorRect(&r, obj->mapColor);
    }
}

bool Map_wrapper::load_map(std::string mapFile, int mapSizeX, int mapSizeY)
{
    mWidth = mapSizeX * TILESIZEPHYSICS;
    mHeight = mapSizeY * TILESIZEPHYSICS;

    if(mapFile!="")
        tiles = import_map(mapFile, mapSizeX, mapSizeY);
    else
        tiles = blank_map(mapSizeX, mapSizeY);
    if (tiles.empty())
        return false;
    return true;
}

bool Map_wrapper::load_blocks(std::string mapFile, int mapSizeX, int mapSizeY)
{
    blocks = import_map(mapFile, mapSizeX, mapSizeY, true);
    if (blocks.empty())
        return false;
    return true;
}

void Map_wrapper::add_sprite_property(int posX, int posY, SDL_Color col, uint health)
{
    std::string type = std::string(1, (char)'a'+posX) + std::string(1, (char)'a'+posY);
    add_sprite_property(type, col, health);
}

void Map_wrapper::add_sprite_property(std::string type, SDL_Color col, uint health)
{
    if(type.size()!=2) throw std::runtime_error("type of sprite has to be string length 2!");
    mappingColor.at(type) = col;
    mappingHealth.at(type) = health;
}

void Map_wrapper::screen_position(SDL_Rect &screenRect, SDL_Rect &viewPort, Object &obj)
{
    obj.positionScreen.x = viewPort.x + viewPort.w / 2;
    obj.positionScreen.y = viewPort.y + viewPort.h / 2;

    screenRect.x = obj.position().x * scaleRender - obj.positionScreen.x;
    if (screenRect.x < 0)
    {
        screenRect.x = 0;
        obj.positionScreen.x = obj.position().x * scaleRender;
    }
    else if (screenRect.x + (viewPort.x + viewPort.w) > mWidth * scaleRender && mWidth * scaleRender > (viewPort.x + viewPort.w))
    {
        screenRect.x = mWidth * scaleRender - (viewPort.x + viewPort.w);
        obj.positionScreen.x = (viewPort.x + viewPort.w) - (mWidth - obj.position().x) * scaleRender;
    }

    screenRect.y = obj.position().y * scaleRender - obj.positionScreen.y;
    if (screenRect.y < 0)
    {
        screenRect.y = 0;
        obj.positionScreen.y = obj.position().y * scaleRender;
    }
    else if (screenRect.y + (viewPort.y + viewPort.h) > mHeight * scaleRender && mHeight * scaleRender > (viewPort.y + viewPort.h))
    {
        screenRect.y = mHeight * scaleRender - (viewPort.y + viewPort.h);
        obj.positionScreen.y = (viewPort.y + viewPort.h) - (mHeight - obj.position().y) * scaleRender;
    }

    screenRect.w = viewPort.w;
    screenRect.h = viewPort.h;
}

std::vector<Object> Map_wrapper::blank_map(int _mapSizeX, int _mapSizeY, bool setHealth)
{
    mapSizeX = _mapSizeX;
    mapSizeY = _mapSizeY;
    int x = 0;
    int y = 0;
    std::vector<Object> output;

    std::string tileType = "aa";
    for (int i = 0; i < mapSizeX * mapSizeY; ++i)
    {
        if (tileType != "00")
        {
            output.push_back(Object(x, y));
            output.back().clip = mappingClips.at(tileType);
            output.back().mapColor = mappingColor.at(tileType);
            output.back().image = image;
            if (setHealth && mappingHealth.at(tileType) != 0)
                output.back().set_health(mappingHealth.at(tileType));
        }

        x += base::TILESIZEPHYSICS;
        if (x >= mapSizeX * base::TILESIZEPHYSICS)
        {
            x = 0;
            y += base::TILESIZEPHYSICS;
        }
    }
    return output;
}

std::vector<Object> Map_wrapper::import_map(std::string mapFile, int _mapSizeX, int _mapSizeY, bool setHealth)
{
    mapSizeX = _mapSizeX;
    mapSizeY = _mapSizeY;
    int x = 0;
    int y = 0;
    std::vector<Object> output;

    std::ifstream mapStream(mapFile);
    if (mapStream.fail())
        throw std::runtime_error("Loading of map file failed!");
    else
    {
        std::string tileType = "00";
        for (int i = 0; i < mapSizeX * mapSizeY; ++i)
        {
            mapStream >> tileType;
            if (mapStream.fail())
            {
                printf("Error loading map: Unexpected end of file!\n");
                break;
            }

            if (tileType != "00")
            {
                output.push_back(Object(x, y));
                output.back().clip = mappingClips.at(tileType);
                output.back().mapColor = mappingColor.at(tileType);
                output.back().image = image;
                if(setHealth && mappingHealth.at(tileType)!=0)
                    output.back().set_health(mappingHealth.at(tileType));
            }

            x += base::TILESIZEPHYSICS;
            if (x >= mapSizeX * base::TILESIZEPHYSICS)
            {
                x = 0;
                y += base::TILESIZEPHYSICS;
            }
        }
    }
    return output;
}