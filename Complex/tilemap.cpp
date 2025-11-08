#include "tilemap.h"
#include <iostream>
#include <fstream>
#include <sstream>

Chunk::Chunk(int x, int y, uint _size) : size(_size)
{
    tiles.reserve(size * size);
    for (uint i = 0; i < size; i++)
        for (uint j = 0; j < size; j++)
            tiles.push_back(
                Block(
                    x + i * base::TILESIZEPHYSICS,
                    y + j * base::TILESIZEPHYSICS)
            );

    boundBox = {
        x, 
        y, 
        static_cast<int>(size * base::TILESIZEPHYSICS),
        static_cast<int>(size * base::TILESIZEPHYSICS)
    };
}

void Chunk::import(const std::string &mapFile, Sprites &sprites)
{
    std::ifstream mapStream(mapFile);
    if (mapStream.fail())
        throw std::runtime_error("Loading of map file " + mapFile + " failed!");

    std::string line;
    uint nTile{0};
    while (getline(mapStream, line))
    {
        std::stringstream lineStream(line);
        std::string tile_name;

        while (lineStream >> tile_name)
        {

            if (tile_name == "NULL")
            {
                continue;
            }

            sprites.at(tile_name).apply(tiles.at(nTile++));

            if(nTile >> size) throw std::runtime_error("Map file " + mapFile + " is too big!");
        }
    }    
}

Map_wrapper::Map_wrapper()
{
}

void Map_wrapper::render_map(Window &window, SDL_Rect &mapPosition, double renderScale)
{
    for (auto &tile : tiles)
    {
        if (tile.image)
            tile.plot(window, mapPosition, renderScale);
    }
}

void Map_wrapper::render_minimap(Window &window, SDL_Rect &minimapPosition, double renderScale, const std::vector<Block *> &objects)
{
    for (auto &tile : tiles)
    {
        if (!tile.on_screen(minimapPosition, renderScale))
            return;
        SDL_Rect renderRect = base::toScreen(minimapPosition, tile.hitbox, renderScale);
        window.drawColorRect(&renderRect, tile.mapColor);
    }

    for (auto &obj : objects)
    {
        if (!obj->on_screen(minimapPosition, renderScale))
            return;
        SDL_Rect renderRect = base::toScreen(minimapPosition, obj->hitbox, renderScale);
        window.drawColorRect(&renderRect, obj->mapColor);
    }
}

std::vector<Block *> Map_wrapper::map_border_colision()
{
    // TODO: define vector directly! No reason to have these
    // as individual variables
    return {&borderLeft, &borderRight, &borderTop, &borderBottom};
}

void Map_wrapper::screen_position(SDL_Rect &worldCoordinatesOnScreen, SDL_Rect &viewPort, Block &obj, double renderScale)
{
    SDL_Rect objPositionScreen;
    objPositionScreen.x = viewPort.w / 2;
    objPositionScreen.y = viewPort.h / 2;

    worldCoordinatesOnScreen.x = obj.position().x * renderScale - objPositionScreen.x;
    worldCoordinatesOnScreen.y = obj.position().y * renderScale - objPositionScreen.y;

    if (worldCoordinatesOnScreen.x < 0)
    {
        worldCoordinatesOnScreen.x = 0;
    }
    else if (worldCoordinatesOnScreen.x + (viewPort.w) > mapWidth * renderScale && mapWidth * renderScale > (viewPort.w))
    {
        worldCoordinatesOnScreen.x = mapWidth * renderScale - (viewPort.w);
    }

    if (worldCoordinatesOnScreen.y < 0)
    {
        worldCoordinatesOnScreen.y = 0;
    }
    else if (worldCoordinatesOnScreen.y + (viewPort.h) > mapHeight * renderScale && mapHeight * renderScale > (viewPort.h))
    {
        worldCoordinatesOnScreen.y = mapHeight * renderScale - (viewPort.h);
    }

    worldCoordinatesOnScreen.w = viewPort.w;
    worldCoordinatesOnScreen.h = viewPort.h;
}

void Map_wrapper::adjust_screen()
{
    mapWidth = nTileX * base::TILESIZEPHYSICS;
    mapHeight = nTileY * base::TILESIZEPHYSICS;
    borderLeft.hitbox = {(int)-base::TILESIZEPHYSICS, (int)-base::TILESIZEPHYSICS,
                         (int)base::TILESIZEPHYSICS, (int)base::TILESIZEPHYSICS * (nTileX + 2)};
    borderRight.hitbox = {(int)base::TILESIZEPHYSICS * nTileX, (int)-base::TILESIZEPHYSICS,
                          (int)base::TILESIZEPHYSICS, (int)base::TILESIZEPHYSICS * (nTileX + 2)};
    borderTop.hitbox = {0, (int)base::TILESIZEPHYSICS * nTileY, (int)base::TILESIZEPHYSICS * nTileY, (int)base::TILESIZEPHYSICS};
    borderBottom.hitbox = {0, (int)-base::TILESIZEPHYSICS, (int)base::TILESIZEPHYSICS * nTileX, (int)base::TILESIZEPHYSICS};
}

void Map_wrapper::blank_map(int _mapSizeX, int _mapSizeY)
{
    nTileX = _mapSizeX;
    nTileY = _mapSizeY;
    uint x = 0;
    uint y = 0;

    for (int i = 0; i < nTileX * nTileY; ++i)
    {
        tiles.emplace_back(x, y);
        x += base::TILESIZEPHYSICS;
        if (x >= nTileX * base::TILESIZEPHYSICS)
        {
            x = 0;
            y += base::TILESIZEPHYSICS;
        }
    }
    adjust_screen();
}

void Map_wrapper::import_map(const std::string &mapFile, Sprites &sprites)
{
    nTileX = 0;
    nTileY = 0;
    uint x = 0;
    uint y = 0;

    std::ifstream mapStream(mapFile);
    if (mapStream.fail())
        throw std::runtime_error("Loading of map file " + mapFile + " failed!");

    std::string line;
    while (getline(mapStream, line))
    {
        nTileY++;

        std::stringstream lineStream(line);
        std::string tile_name;
        nTileX = 0;

        while (lineStream >> tile_name)
        {
            nTileX++;

            if (tile_name == "NULL")
            {
                x += base::TILESIZEPHYSICS;
                continue;
            }

            tiles.emplace_back(x, y);
            sprites.at(tile_name).apply(tiles.back());
            x += base::TILESIZEPHYSICS;
        }
        y += base::TILESIZEPHYSICS;
        x = 0;
        // TODO: no need to compute each time ...
    }
    adjust_screen();
}

void Map_wrapper::export_map(const std::string &mapFile)
{
    std::ofstream mapStream(mapFile);
    if (mapStream.fail())
        throw std::runtime_error("Creating map file " + mapFile + " failed!");
    for (auto &tile : tiles)
    {
        mapStream << tile.hitbox.x << " ";
    }
    mapStream << std::endl;
}

std::vector<Block *> &Map_wrapper::get_tile_pointers()
{
    tilePointers.reserve(tiles.size());
    for (auto &tile : tiles)
        tilePointers.push_back(&tile);
    return tilePointers;
}
