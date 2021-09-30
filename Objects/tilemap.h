#ifndef MAP_H
#define MAP_H

#include "gameplay.h"
#include "SDL_wrapper.h"
#include "object.h"

#include <vector>
#include <map>

class Map_wrapper : public gameplay
{
public:
    Map_wrapper(SDL_Rect border = {0, 0, TILESIZEINPUT*12, 0});
    ~Map_wrapper(){}

    void render_minimap( SDL_wrapper & wrapper, std::vector<Object*> & objects );
    void render_map( SDL_wrapper & wrapper, SDL_Rect & mapPosition );
    void add_sprite(int type, int posX, int posY , SDL_Color col, uint health = 0);
    bool load_map( std::string mapFile, int mapSizeX, int mapSizeY );
    bool load_blocks( std::string mapFile, int mapSizeX, int mapSizeY );
    SDL_Rect screen;
    double scale;
    std::map<int,SDL_Color> mappingColor;
    std::map<int,uint> mappingHealth;
    std::shared_ptr<std::map<int,SDL_Rect>> clips;
    std::vector<std::unique_ptr<Object>> tiles;
    std::vector<std::unique_ptr<Object>> blocks;
    std::shared_ptr<IMG_wrapper> image;

// TODO: General problem, most stuff public!
private:
    SDL_Rect gameplayBorder;
    SDL_Rect minimapBorder;
    SDL_Rect viewPort;
};
#endif // MAP_H
