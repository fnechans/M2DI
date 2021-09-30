#ifndef LEVEL_H
#define LEVEL_H

#include "gameplay.h"
#include "SDL_wrapper.h"
#include "tilemap.h"
#include "melee.h"
#include "IMG_wrapper.h"
#include "astar.h"
#include "button.h"
#include "player.h"
#include "character.h"

#include <utility>

// Goal of Level is that player does not have to
// interact with any Object unless really necessary
class Level : gameplay
{
public:
    Level(SDL_wrapper* _wrapper);
    void bake(); // called before loop!

    void preprocess();
    void evaluate(SDL_Event& event);
    bool screenClick = false;
    void process();
    void plot();

    Map_wrapper& get_map(){return curMap;}
    void set_map_image(std::string name){curMap.image = images[name];}

    // Melee
    void add_melee(std::string name, Melee melee)
    {
        melees.emplace(name,melee);
     //   melees.insert(std::make_pair<std::string,Melee>(name, melee));
       // Melee m(melee);
        //melees[name] = Melee(melee);
    }
    Melee& get_melee(std::string name)
    {
        return melees.at(name);
    }
    // TODO: proper casting! not only here
    void eval_melee(std::string charName, std::string meleeName)
    {
        Movable* chr = (charName=="player") ? (Movable*) &player : (Movable*) &characters.at(charName);
        melees.at(meleeName).evaluate(chr, collisionObjects);
    }

    // Images
    void add_image(std::string name, std::string imagePath)
    {
        images.emplace(name, std::make_shared<IMG_wrapper>());
        images[name]->load_media(imagePath.c_str());
    }

    // Player
    PPlayer * get_player(){return &player;}
    void set_player_position(int x, int y)
    {
        player.position.x = x*TILESIZE; 
        player.position.y = y*TILESIZE; 
    }
    void set_player_image(std::string name, SDL_Color color)
    {
        player.image = images[name];
        player.mapColor = color;
    }
    void add_player_animation(std::string name, Animation animation, std::string imgName)
    {
        player.animations[name] = std::move(animation);
        player.animations[name].image = images[imgName];
    }
    void set_player_property(std::string property, int value)
    {
        player.property[property] = value;
    }

    // Characters
    void add_character(std::string name, int x, int y)
    {
        if(name=="player")
        {
            throw std::runtime_error("Name player is reserved!");
        }
        characters.emplace(name, Character(x*TILESIZE,y*TILESIZE));
        // TMP
        characters[name].set_health(10);
    }
    void add_character_animation(std::string name, std::string aniName, Animation animation, std::string imgName)
    {
        characters[name].animations[aniName] = std::move(animation);
        characters[name].animations[aniName].image = images[imgName];
    }
    void copy_character_animation(std::string fromName, std::string targetName)
    {
        if(fromName==targetName)
        {
            throw std::runtime_error("fromName and targetName are the same!");
        }
        auto & tarChar = characters.at(targetName);
        if(fromName=="player") tarChar.copy_animation(player);
        else tarChar.copy_animation(characters.at(fromName));
    }
    void set_character_target(std::string name, std::string targetName)
    {
        if(name==targetName)
        {
            throw std::runtime_error("name and targetName are the same!");
        }
        auto & ch = characters.at(name);
        if(targetName=="player") ch.target = &player;
        else ch.target = &characters.at(targetName);
    }
    void set_character_image(std::string name, std::string imgName, SDL_Color color)
    {
        characters[name].image = images[imgName];
        characters[name].mapColor = color;
    }
    
    std::string get_direction(std::string name)
    {
        if(name=="player") return Object::dirName[player.dir];
        else return Object::dirName[characters.at(name).dir];
    }
    void set_character_property(std::string name, std::string property, int value)
    {
        characters[name].property[property] = value;
    }

    // object-collection getter:
    std::map<std::string,Character>& get_chars(){return characters;}
    std::vector<Object*>& get_collisionObjects(){return collisionObjects;}

    // AI related
    bool tick(uint frequency){return AIclick%frequency==0;}
    void init_astar()
    {
        std::vector<Object*> astarTiles;
        for( auto& t : curMap.tiles ) astarTiles.push_back(t.get());
        acko = std::make_unique<AStar<Object*>>(astarTiles);
    }
    bool pause = false; // is level paused?
private:
    SDL_wrapper * wrapper;
    Map_wrapper curMap;
    std::vector<Object*> collisionObjects;
    SDL_Rect screenRect;
    button bScreen;

    std::map<std::string,Melee> melees;
    std::map<std::string,std::shared_ptr<IMG_wrapper>> images;

    PPlayer player;
    std::map<std::string,Character> characters;
    std::unique_ptr<AStar<Object*>> acko = nullptr;

    int AIclick = 0; // how often is AI updated
    bool baked = false; // defines if level is ready
    // (finalizes the level after adding all objects)
};


#endif