#ifndef LEVEL_H
#define LEVEL_H

#include "Tools/astar.h"
#include "Basics/viewport.h"
#include "Complex/tilemap.h"
#include "Damage/dmgr_inst.h"
#include "SDL_wrapper/IMG_wrapper.h"
#include "Basics/button.h"
#include "Basics/vision_cone.h"
#include "Complex/character.h"
#include "Managers/object_manager.hpp"

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
    void move_chars(double DELTA_T);
    void plot_map();
    void plot();

    void set_map(SDL_Rect border = {0, 0, base::TILESIZEINPUT * 12, 0})
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
        obj->animations.emplace(aniName, std::move(animation));
        obj->animations[aniName].image = images[imgName];
    }

    // object-collection getter:
    std::vector<Block *> &get_collision_objects() { return collisionObjects; }
    std::vector<Block *> &get_obscuring_objects() { return obscuringObjects; }
    std::vector<Object *> &get_damagable_objects() { return damagableObjects; }

    bool pause = false; // is level paused?
    SDL_Rect screenRect;

    button bScreen;

    Object_manager<Character> characters;
    Object_manager<Character> grenades;
private:
    std::unique_ptr<Map_wrapper> curMap;
    // TODO: might be better to have single vector
    // and make functions which use them check
    // flags defined in the Block?
    std::vector<Block *> collisionObjects;
    std::vector<Block *> obscuringObjects;
    std::vector<Object *> damagableObjects;

    std::map<std::string, std::shared_ptr<IMG_wrapper>> images;

};

#endif