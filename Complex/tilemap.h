#ifndef MAP_H
#define MAP_H

#include "Tools/base.h"
#include "Basics/block.h"

#include <vector>
#include <map>
#include <cstring>

class Sprite
{
public:
    Sprite(IMG_wrapper* img, SDL_Rect clip, SDL_Color col, bool obs=true) : image(img), clip(clip), color(col), obscure(obs) {}
    IMG_wrapper* image;
    SDL_Rect clip;
    SDL_Color color;
    bool obscure;
};

class Sprites
{
public:
    Sprites() {}
    ~Sprites() {}
    void add(std::string name, IMG_wrapper* img, SDL_Rect clip, SDL_Color col, bool obs)
    { sprites.emplace(name, Sprite(img, clip, col, obs)); }

    Sprite& at(std::string name) { return sprites.at(name); }
    std::map<std::string, Sprite> sprites;
};

class Map_wrapper
{
public:
    Map_wrapper();
    ~Map_wrapper() {}

    void init(int xMax, int yMax, int tileSize);
    void render_map(Window &wrapper, SDL_Rect &mapPosition, double renderScale);
    void render_minimap(Window &window, SDL_Rect &minimapViewPort, double renderSCale, const std::vector<Block *> &objects = {});
    void adjust_screen();
    void screen_position(SDL_Rect &worldCoordinatesOnScreen, SDL_Rect &viewPort, Block &block, double renderScale);
    void import_map(std::string mapFile, Sprites& sprites);
    void blank_map(int mapSizeX, int mapSizeY);
    std::vector<Block*>& get_tile_pointers();
    std::vector<Block> tiles;

    // TODO: General problem, most stuff public!
    int nTileX;
    int nTileY;

    uint width(){return mapWidth;}
    uint height(){return mapHeight;}

    std::vector<Block*> map_border_colision();
private:
    SDL_Rect minimapViewPort;

    // Map dimension constants
    uint mapWidth;
    uint mapHeight;
    // Borders to restrict movement
    Block borderLeft, borderRight, borderTop, borderBottom;
    
    std::vector<Block*> tilePointers;
};

SDL_Rect get_tile_draw_rect(Block const &block, double& scale);



#endif // MAP_H
