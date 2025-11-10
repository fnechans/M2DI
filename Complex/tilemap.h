#ifndef MAP_H
#define MAP_H

#include "Tools/base.h"
#include "Basics/block.h"
#include "chunk.h"

#include <vector>
#include <map>
#include <cstring>

class Map_wrapper
{
public:
    Map_wrapper() {}
    ~Map_wrapper() {}

    void add_chunk(Chunk chunk);

    void render_map(Window &wrapper, SDL_Rect &mapPosition, double renderScale);
    void render_minimap(Window &window, SDL_Rect &minimapPosition, double renderSCale, const std::vector<Block *> &objects = {});
    void adjust_screen();
    void screen_position(SDL_Rect &worldCoordinatesOnScreen, SDL_Rect &viewPort, Block &block, double renderScale);
    std::vector<Block*>& get_tile_pointers(bool activeOnly = false);

    std::vector<Block*>& map_border_colision();
private:
    SDL_Rect minimapViewPort;

    // Bound box is xmin, ymin, xmax, ymax!!!
    SDL_Rect boundBox{0, 0, 0, 0};
    // Borders to restrict movement
    Block borderLeft, borderRight, borderTop, borderBottom;
    std::vector<Block*> border;

    std::vector<Block*> tilePointers;

    std::vector<Chunk> chunks;
};

SDL_Rect get_tile_draw_rect(Block const &block, double& scale);



#endif // MAP_H
