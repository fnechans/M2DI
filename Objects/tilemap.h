#ifndef MAP_H
#define MAP_H

#include "base.h"
#include "object.h"

#include <vector>
#include <map>
#include <cstring>


class Map_wrapper : public base
{
public:
    Map_wrapper(SDL_Rect border = {0, 0, TILESIZEINPUT * 12, 0});
    ~Map_wrapper() {}

    void init(uint xMax, uint yMax);
    void render_minimap(Window &wrapper, std::vector<Object *> &objects);
    void render_map(Window &wrapper, SDL_Rect &mapPosition);
    void add_sprite_property(int posX, int posY, SDL_Color col, uint health = 0);
    void add_sprite_property(std::string, SDL_Color col, uint health = 0);
    bool load_map(std::string mapFile, int mapSizeX, int mapSizeY);
    bool load_blocks(std::string mapFile, int mapSizeX, int mapSizeY);
    void screen_position(SDL_Rect &screenRect, Object &obj);
    std::vector<Object> import_map(std::string mapFile, int mapSizeX, int mapSizeY, bool set_health = false);
    std::vector<Object> blank_map(int mapSizeX, int mapSizeY, bool set_health = false);
    SDL_Rect screen;
    double scale;
    std::map<std::string, SDL_Color> mappingColor;
    std::map<std::string, uint> mappingHealth;
    std::map<std::string, SDL_Rect> mappingClips;
    std::vector<Object> tiles;
    std::vector<Object> blocks;
    std::shared_ptr<IMG_wrapper> image;

    // TODO: General problem, most stuff public!
    SDL_Rect gameplayScreen;
    uint mapSizeX;
    uint mapSizeY;

private:
    SDL_Rect gameplayBorder;
    SDL_Rect minimapBorder;
    SDL_Rect minimapViewPort;
};

SDL_Rect get_tile_draw_rect(Object const &obj, double& scale);

#endif // MAP_H
