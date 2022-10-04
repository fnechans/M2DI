#ifndef LEVEL_H
#define LEVEL_H

#include "viewport.h"
#include "tilemap.h"
#include "dmgr_inst.h"
#include "IMG_wrapper.h"
#include "astar.h"
#include "button.h"
#include "character.h"

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

    // Characters
    Character *add_character(std::string name, int x, int y)
    {
        characters.emplace(name, Character(x * TILESIZEPHYSICS, y * TILESIZEPHYSICS));
        // TMP
        characters[name].set_health(10);
        return &characters[name];
    }
    void add_character_animation(std::string name, std::string aniName, Animation animation, std::string imgName)
    {
        characters[name].animations[aniName] = std::move(animation);
        characters[name].animations[aniName].image = images[imgName];
    }
    void copy_character_animation(std::string fromName, std::string targetName)
    {
        if (fromName == targetName)
        {
            throw std::runtime_error("fromName and targetName are the same!");
        }
        auto &tarChar = characters.at(targetName);
        tarChar.copy_animation(characters.at(fromName));
    }
    void set_character_target(std::string name, std::string targetName)
    {
        if (name == targetName)
        {
            throw std::runtime_error("name and targetName are the same!");
        }
        auto &ch = characters.at(name);
        ch.target = &characters.at(targetName);
    }
    void set_character_image(std::string name, std::string imgName, SDL_Color color)
    {
        characters[name].image = images[imgName];
        characters[name].mapColor = color;
    }

    std::string get_direction(std::string name)
    {
        return Object::dirName[characters.at(name).dir];
    }
    void set_character_property(std::string name, std::string property, int value)
    {
        characters[name].property[property] = value;
    }

    // object-collection getter:
    std::map<std::string, Character> &get_chars() { return characters; }
    Character &get_char(std::string name) { return characters.at(name); }
    std::vector<Block *> &get_collisionObjects() { return collisionObjects; }
    std::vector<Object *> &get_damagableObjects() { return damagableObjects; }

    bool pause = false; // is level paused?
    SDL_Rect screenRect;

    button bScreen;

private:
    std::unique_ptr<Map_wrapper> curMap;
    std::vector<Block *> collisionObjects;
    std::vector<Object *> damagableObjects;

    std::map<std::string, std::shared_ptr<IMG_wrapper>> images;

    std::map<std::string, Character> characters;
};

#endif