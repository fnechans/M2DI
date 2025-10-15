#include "tilemap.h"
#include <iostream>
#include <fstream>

Map_wrapper::Map_wrapper(SDL_Rect border)
    : gameplayBorder(border)
{
}

void Map_wrapper::init(int xMax, int yMax)
{
    for (int x = 0; x < xMax; x++)
    {
        for (int y = 0; y < yMax; y++)
        {
            std::string type = std::string(1, (char)'a' + x) + std::string(1, (char)'a' + y);
            mappingClips[type] = {x * (int)base::TILESIZEINPUT, y * (int)base::TILESIZEINPUT,
                                  (int)base::TILESIZEINPUT, (int)base::TILESIZEINPUT};
            mappingColor[type] = {0, 0, 0, 0};
            mappingObscure[type] = true;
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

SDL_Rect get_tile_draw_rect(Block const &obj, double &scale)
{
    return {(int)(obj.hitbox.x / scale),
            (int)(obj.hitbox.y / scale),
            (int)(base::TILESIZEPHYSICS / scale),
            (int)(base::TILESIZEPHYSICS / scale)};
}

void Map_wrapper::render_minimap(Window &window, std::vector<Block *> &objects)
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
    mWidth = mapSizeX * base::TILESIZEPHYSICS;
    mHeight = mapSizeY * base::TILESIZEPHYSICS;
    borderLeft.hitbox = {(int)-base::TILESIZEPHYSICS, (int)-base::TILESIZEPHYSICS, (int)base::TILESIZEPHYSICS, (int)base::TILESIZEPHYSICS * (mapSizeX + 2)};
    borderRight.hitbox = {(int)base::TILESIZEPHYSICS * mapSizeX, (int)-base::TILESIZEPHYSICS, (int)base::TILESIZEPHYSICS, (int)base::TILESIZEPHYSICS * (mapSizeX + 2)};
    borderTop.hitbox = {0, (int)base::TILESIZEPHYSICS * mapSizeY, (int)base::TILESIZEPHYSICS * mapSizeX, (int)base::TILESIZEPHYSICS};
    borderBottom.hitbox = {0, (int)-base::TILESIZEPHYSICS, (int)base::TILESIZEPHYSICS * mapSizeX, (int)base::TILESIZEPHYSICS};

    if (mapFile != "")
        tiles = import_map(mapFile, mapSizeX, mapSizeY);
    else
        tiles = blank_map(mapSizeX, mapSizeY);
    if (tiles.empty())
        return false;
    return true;
}

bool Map_wrapper::load_blocks(std::string mapFile, int mapSizeX, int mapSizeY)
{
    blocks = import_map(mapFile, mapSizeX, mapSizeY);
    if (blocks.empty())
        return false;
    return true;
}

void Map_wrapper::add_sprite_property(int posX, int posY, SDL_Color col, bool obscures)
{
    std::string type = std::string(1, (char)'a' + posX) + std::string(1, (char)'a' + posY);
    add_sprite_property2(type, col, obscures);
}

void Map_wrapper::add_sprite_property2(std::string type, SDL_Color col, bool obscures)
{
    if (type.size() != 2)
        throw std::runtime_error("type of sprite has to be string length 2!");
    mappingColor.at(type) = col;
    mappingObscure.at(type) = obscures;
}

std::vector<Block *> Map_wrapper::map_border_colision()
{
    // TODO: define vector directly! No reason to have these
    // as individual variables
    return {&borderLeft, &borderRight, &borderTop, &borderBottom};
}

void Map_wrapper::screen_position(SDL_Rect &screenRect, SDL_Rect &viewPort, Block &obj)
{
    obj.positionScreen.x = viewPort.x + viewPort.w / 2;
    obj.positionScreen.y = viewPort.y + viewPort.h / 2;

    screenRect.x = obj.position().x * base::scaleRender() - obj.positionScreen.x;
    screenRect.y = obj.position().y * base::scaleRender() - obj.positionScreen.y;

    if (screenRect.x < 0)
    {
        screenRect.x = 0;
        obj.positionScreen.x = obj.position().x * base::scaleRender();
    }
    else if (screenRect.x + (viewPort.x + viewPort.w) > mWidth * base::scaleRender() && mWidth * base::scaleRender() > (viewPort.x + viewPort.w))
    {
        screenRect.x = mWidth * base::scaleRender() - (viewPort.x + viewPort.w);
        obj.positionScreen.x = (viewPort.x + viewPort.w) - (mWidth - obj.position().x) * base::scaleRender();
    }

    if (screenRect.y < 0)
    {
        screenRect.y = 0;
        obj.positionScreen.y = obj.position().y * base::scaleRender();
    }
    else if (screenRect.y + (viewPort.y + viewPort.h) > mHeight * base::scaleRender() && mHeight * base::scaleRender() > (viewPort.y + viewPort.h))
    {
        screenRect.y = mHeight * base::scaleRender() - (viewPort.y + viewPort.h);
        obj.positionScreen.y = (viewPort.y + viewPort.h) - (mHeight - obj.position().y) * base::scaleRender();
    }

    screenRect.w = viewPort.w;
    screenRect.h = viewPort.h;
}

std::vector<Block> Map_wrapper::blank_map(int _mapSizeX, int _mapSizeY)
{
    nTileX = _mapSizeX;
    nTileY = _mapSizeY;
    uint x = 0;
    uint y = 0;
    std::vector<Block> output;

    std::string tileType = "aa";
    for (uint i = 0; i < nTileX * nTileY; ++i)
    {
        if (tileType != "00")
        {
            output.emplace_back(x, y);
            output.back().clip = mappingClips.at(tileType);
            output.back().mapColor = mappingColor.at(tileType);
            output.back().obscuresVision = mappingObscure.at(tileType);
            output.back().image = image;
        }

        x += base::TILESIZEPHYSICS;
        if (x >= nTileX * base::TILESIZEPHYSICS)
        {
            x = 0;
            y += base::TILESIZEPHYSICS;
        }
    }
    return output;
}

std::vector<Block> Map_wrapper::import_map(std::string mapFile, int _mapSizeX, int _mapSizeY)
{
    nTileX = _mapSizeX;
    nTileY = _mapSizeY;
    uint x = 0;
    uint y = 0;
    std::vector<Block> output;

    std::ifstream mapStream(mapFile);
    if (mapStream.fail())
        throw std::runtime_error("Loading of map file failed!");
    else
    {
        std::string tileType = "00";
        for (uint i = 0; i < nTileX * nTileY; ++i)
        {
            mapStream >> tileType;
            if (mapStream.fail())
            {
                printf("Error loading map: Unexpected end of file!\n");
                break;
            }

            if (tileType != "00")
            {
                output.emplace_back(x, y);
                output.back().clip = mappingClips.at(tileType);
                output.back().mapColor = mappingColor.at(tileType);
                output.back().obscuresVision = mappingObscure.at(tileType);
                output.back().image = image;
            }

            x += base::TILESIZEPHYSICS;
            if (x >= nTileX * base::TILESIZEPHYSICS)
            {
                x = 0;
                y += base::TILESIZEPHYSICS;
            }
        }
    }
    return output;
}