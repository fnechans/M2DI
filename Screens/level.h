#ifndef LEVEL_H
#define LEVEL_H

#include "viewport.h"
#include "tilemap.h"
#include "dmgr_inst.h"
#include "IMG_wrapper.h"
#include "astar.h"
#include "button.h"
#include "character.h"
#include "object_manager.h"

#include <utility>

// Goal of Level is that player does not have to
// interact with any Object unless really necessary
class Level : public Viewport
{
public:
    Level(Window *win, Position pos = WHOLE, SDL_Rect bor = {0, 0, 0, 0});
    Level(const Level *) = delete;
    void bake(); // called before loop!

    void reset();
    bool evaluate(SDL_Event &event); // returns true if event relevant to speed up/avoid multiple evals
    bool screenClick = false;
    SDL_Rect mousePositionScreen;
    void move_chars();
    void plot();

    void set_map(SDL_Rect border = {0, 0, TILESIZEINPUT * 12, 0})
    {
        curMap = std::make_unique<Map_wrapper>(border);
    }
    Map_wrapper &get_map() { return *curMap; }
    void set_map_image(std::string name) { curMap->image = images[name]; }

     // Images
    void add_image(std::string name, std::string imagePath)
    {
        images.emplace(name, std::make_shared<IMG_wrapper>());
        images[name]->load_media(*window, imagePath.c_str());
    }
    std::shared_ptr<IMG_wrapper> get_image(const std::string& name)
    {
        return images.at(name);
    }

    void add_object_animation(Object* obj, std::string aniName, Animation animation, std::string imgName)
    {
        obj->animations[aniName] = std::move(animation);
        obj->animations[aniName].image = images[imgName];
    }

    // object-collection getter:
    std::vector<Block *> &get_collisionObjects() { return collisionObjects; }
    std::vector<Object *> &get_damagableObjects() { return damagableObjects; }

    bool pause = false; // is level paused?
    SDL_Rect screenRect;

    button bScreen;

    Object_manager<Character> characters;
    Object_manager<Character> grenades;
private:
    std::unique_ptr<Map_wrapper> curMap;
    std::vector<Block *> collisionObjects;
    std::vector<Object *> damagableObjects;

    std::map<std::string, std::shared_ptr<IMG_wrapper>> images;

};

#endif