#ifndef MAP_H
#define MAP_H

#include "base.h"
#include "block.h"

#include <vector>
#include <map>
#include <cstring>


class Map_wrapper : public base
{
public:
    Map_wrapper(SDL_Rect border = {0, 0, TILESIZEINPUT * 12, 0});
    ~Map_wrapper() {}

    void init(int xMax, int yMax);
    void render_minimap(Window &wrapper, std::vector<Block *> &block);
    void render_map(Window &wrapper, SDL_Rect &mapPosition);
    void add_sprite_property(int posX, int posY, SDL_Color col);
    void add_sprite_property(std::string, SDL_Color col);
    bool load_map(std::string mapFile, int mapSizeX, int mapSizeY);
    bool load_blocks(std::string mapFile, int mapSizeX, int mapSizeY);
    void screen_position(SDL_Rect &screenRect, SDL_Rect &viewPort, Block &block);
    std::vector<Block> import_map(std::string mapFile, int mapSizeX, int mapSizeY);
    std::vector<Block> blank_map(int mapSizeX, int mapSizeY);
    SDL_Rect screen;
    double scale;
    std::map<std::string, SDL_Color> mappingColor;
    std::map<std::string, SDL_Rect> mappingClips;
    std::vector<Block> tiles;
    std::vector<Block> blocks;
    std::shared_ptr<IMG_wrapper> image;

    // TODO: General problem, most stuff public!
    uint mapSizeX;
    uint mapSizeY;

private:
    SDL_Rect gameplayBorder;
    SDL_Rect minimapBorder;
    SDL_Rect minimapViewPort;
};

SDL_Rect get_tile_draw_rect(Block const &block, double& scale);

#endif // MAP_H
