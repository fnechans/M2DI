#ifndef LEVEL_H
#define LEVEL_H

#include "window.h"
#include "tilemap.h"
#include "melee.h"
#include "IMG_wrapper.h"
#include "astar.h"
#include "button.h"
#include "character.h"

#include <utility>

// Goal of Level is that player does not have to
// interact with any Object unless really necessary
class Level : base
{
public:
    Level(Window *_wrapper);
    Level(const Level *) = delete;
    void bake(); // called before loop!

    void preprocess();
    void evaluate(SDL_Event &event);
    bool screenClick = false;
    void move_chars();
    void plot();

    Map_wrapper &get_map() { return curMap; }
    void set_map_image(std::string name) { curMap.image = images[name]; }

    // Melee
    void add_melee(std::string name, Melee melee)
    {
        melees.emplace(name, melee);
        //   melees.insert(std::make_pair<std::string,Melee>(name, melee));
        // Melee m(melee);
        //melees[name] = Melee(melee);
    }
    Melee &get_melee(std::string name)
    {
        return melees.at(name);
    }
    // TODO: proper casting! not only here
    void eval_melee(std::string charName, std::string meleeName)
    {
        Character *chr = &characters.at(charName);
        melees.at(meleeName).evaluate(chr, collisionObjects);
    }

    // Images
    void add_image(std::string name, std::string imagePath)
    {
        images.emplace(name, std::make_shared<IMG_wrapper>());
        images[name]->load_media(*window, imagePath.c_str());
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
    void add_character_melee(std::string name, std::string meleeName)
    {
        Melee_instance mel(&melees.at(meleeName));
        characters.at(name).melees.emplace(meleeName, mel);
    }
    void set_character_property(std::string name, std::string property, int value)
    {
        characters[name].property[property] = value;
    }

    // object-collection getter:
    std::map<std::string, Character> &get_chars() { return characters; }
    Character &get_char(std::string name) { return characters.at(name); }
    std::vector<Object *> &get_collisionObjects() { return collisionObjects; }

    bool pause = false; // is level paused?
    SDL_Rect screenRect;

private:
    Window *window;
    Map_wrapper curMap;
    std::vector<Object *> collisionObjects;
    button bScreen;

    std::map<std::string, Melee> melees;
    std::map<std::string, std::shared_ptr<IMG_wrapper>> images;

    std::map<std::string, Character> characters;
};

#endif