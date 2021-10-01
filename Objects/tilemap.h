#ifndef MAP_H
#define MAP_H

#include "base.h"
#include "object.h"

#include <vector>
#include <map>

class Map_wrapper : public base
{
public:
    Map_wrapper(SDL_Rect border = {0, 0, TILESIZEINPUT*12, 0});
    ~Map_wrapper(){}

    void render_minimap( Window & wrapper, std::vector<Object*> & objects );
    void render_map( Window & wrapper, SDL_Rect & mapPosition );
    void add_sprite(int type, int posX, int posY , SDL_Color col, uint health = 0);
    bool load_map( std::string mapFile, int mapSizeX, int mapSizeY );
    bool load_blocks( std::string mapFile, int mapSizeX, int mapSizeY );
    void screen_position(SDL_Rect &screenRect, Object& obj);
    SDL_Rect screen;
    double scale;
    std::map<int,SDL_Color> mappingColor;
    std::map<int,uint> mappingHealth;
    std::shared_ptr<std::map<int,SDL_Rect>> clips;
    std::vector<Object> tiles;
    std::vector<Object> blocks;
    std::shared_ptr<IMG_wrapper> image;

// TODO: General problem, most stuff public!
    SDL_Rect gameplayScreen;
private:
    SDL_Rect gameplayBorder;
    SDL_Rect minimapBorder;
    SDL_Rect minimapViewPort;
};
#endif // MAP_H
