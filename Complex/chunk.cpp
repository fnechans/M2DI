#include "chunk.h"

#include <iostream>
#include <fstream>
#include <sstream>

Chunk::Chunk(int x, int y, uint _size) : size(_size)
{

    x *= base::TILESIZEPHYSICS * size;
    y *= base::TILESIZEPHYSICS * size;

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
        x + static_cast<int>(size * base::TILESIZEPHYSICS),
        y + static_cast<int>(size * base::TILESIZEPHYSICS)
    };
}



Chunk::Chunk(int xInit, int yInit, const std::string &mapFile, Sprites &sprites, uint _size) : size(_size)
{
    std::cout << "Loading map file: " << mapFile << " at " << xInit << ", " << yInit << std::endl;
    std::ifstream mapStream(mapFile);
    if (mapStream.fail())
        throw std::runtime_error("Loading of map file " + mapFile + " failed!");
    xInit *= base::TILESIZEPHYSICS * size;
    yInit *= base::TILESIZEPHYSICS * size;

    std::string line;
    uint nTileX = 0;
    uint nTileY = 0;
    int x = xInit;
    int y = yInit;

    while (getline(mapStream, line))
    {
        nTileY++;
        if(nTileY > size)
        {
            std::cout<< "Too many lines in map file!" << std::endl;
            break;
        }

        std::stringstream lineStream(line);
        std::string tile_name;
        nTileX = 0;

        while (lineStream >> tile_name)
        {
            nTileX++;
            if(nTileX > size)
            {
                std::cout<< "Too many columns in map file!" << std::endl;
                break;
            }

            if (tile_name == "NULL")
            {
                x += base::TILESIZEPHYSICS;
                continue;
            }

            tiles.emplace_back(x, y);
            tiles.back().sprite = &sprites.at(tile_name);
            x += base::TILESIZEPHYSICS;
        }
        y += base::TILESIZEPHYSICS;
        x = xInit;
    }
    boundBox = {
        xInit,
        yInit,
        xInit + static_cast<int>(size * base::TILESIZEPHYSICS),
        yInit + static_cast<int>(size * base::TILESIZEPHYSICS)
    };
}


void Chunk::plot(Window &window, SDL_Rect &screen, double renderScale)
{
    for (auto &tile : tiles)
        tile.plot(window, screen, renderScale);
}

void Chunk::plot_minimap(Window &window, SDL_Rect &screen, double renderScale)
{
    for (auto &tile : tiles)
    {
        if (tile.sprite == nullptr || !tile.on_screen(screen, renderScale))
            continue;
        SDL_Rect renderRect = base::toScreen(screen, tile.hitbox, renderScale);
        window.drawColorRect(&renderRect, tile.sprite->color);
    }
}