#ifndef CHUNK_H
#define CHUNK_H

#include "Tools/base.h"
#include "Basics/block.h"

#include <vector>
#include <map>
#include <cstring>

class Chunk
{
public:
    Chunk(int x, int y, uint size = 16);
    Chunk(int x, int y, const std::string& mapFile, Sprites& sprites, uint size = 16);
    std::vector<Block> tiles;
    uint size;
    void plot(Window &window, SDL_Rect &screen, double renderScale);
    void plot_minimap(Window &window, SDL_Rect &screen, double renderScale);
    bool active = true;
    // Bound box is xmin, ymin, xmax, ymax!!!
    SDL_Rect boundBox;
};

#endif // CHUNK_H
