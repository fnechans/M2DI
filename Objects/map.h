#ifndef MAP_H
#define MAP_H

#include "base.h"
#include "SDL_wrapper.h"
#include "object.h"

#include <vector>
#include <map>

namespace M2DI {
class Map_wrapper : public base
{
public:
    Map_wrapper();

    void render_minimap( SDL_wrapper & wrapper, std::vector<std::shared_ptr<object>> & objects );
    void render_map( SDL_wrapper & wrapper, SDL_Rect & mapPosition );
    void add_sprite(int type, int posX, int posY , SDL_Color col);
    bool load_map( std::string mapFile, int mapSizeX, int mapSizeY );
    SDL_Rect minimap;
    SDL_Rect screen;
    int scale;
    std::map<int,SDL_Color> mappingColor;
    std::shared_ptr<std::map<int,SDL_Rect>> clips;
    std::vector<std::shared_ptr<object>> tiles;
    std::shared_ptr<IMG_wrapper> image;
};
}
#endif // MAP_H
