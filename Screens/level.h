#ifndef LEVEL_H
#define LEVEL_H

#include "Tools/astar.h"
#include "Basics/viewport.h"
#include "Complex/tilemap.h"
#include "Damage/dmgr_inst.h"
#include "SDL_wrapper/IMG_wrapper.h"
#include "Basics/button.h"
#include "Basics/vision_cone.h"
#include "Managers/object_manager.hpp"

#include <utility>

using ObjectManager = ObjManager<Object>;
// Goal of Level is that player does not have to
// interact with any Object unless really necessary
class Level : public Viewport, public HasProperties
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
    void plot();

    void set_map()
    {
        currentMap = std::make_unique<Map_wrapper>();
        currentBlocks = std::make_unique<Map_wrapper>();
    }
    Map_wrapper &get_map() { return *currentMap; }
    Map_wrapper &get_blocks() { return *currentBlocks; }

    double renderScale = 40. / base::TILESIZEPHYSICS;
    double renderScaleMinimap = 10. / base::TILESIZEPHYSICS;
    void set_map_screen_position(Block *target)
    {
        currentMap->screen_position(worldCoordinatesOnScreen, viewPort, *target, renderScale);
        activeWorldCoordinates = {
            static_cast<int>(worldCoordinatesOnScreen.x / renderScale), 
            static_cast<int>(worldCoordinatesOnScreen.y / renderScale), 
            static_cast<int>(worldCoordinatesOnScreen.w / renderScale), 
            static_cast<int>(worldCoordinatesOnScreen.h / renderScale)
        };
        if (minimap)
            currentMap->screen_position(worldCoordinatesOnMinimap, minimap->viewPort, *target, renderScaleMinimap);
    }
    void set_ai()
    {
        ai.init(currentMap->get_tile_pointers());
        ai_active = true;
    }
    void follow_ai()
    {
        ai.increment();
        for (auto chr : characters)
        {
            // TODO: implement "next to" properly
            SDL_Rect hitbox_plus_one = 
            {
                chr->hitbox.x - 1,
                chr->hitbox.y - 1,
                chr->hitbox.w + 2,
                chr->hitbox.h + 2
            };
            if (!chr->target)
                continue;
            if (SDL_HasIntersection(&chr->target->hitbox, &hitbox_plus_one)){
                // After this, follow_path still needed to reset velocity
                chr->path.clear();
            }
            else if (ai.tick(50))
            {
                auto tmp = ai.acko->find_path(chr, chr->target, get_collision_objects());
                if (!tmp.empty())
                    chr->path = tmp;
            }
            chr->follow_path(get_collision_objects());
        }
    }

    void add_minimap(Position pos, SDL_Rect bor) { minimap = std::make_unique<Viewport>(window, pos, bor); }

    // object-collection getter:
    std::vector<Block *> &get_collision_objects() { return collisionObjects; }
    std::vector<Block *> &get_obscuring_objects() { return obscuringObjects; }
    std::vector<Object *> &get_damagable_objects() { return damagableObjects; }

    bool pause = false; // is level paused?

    ObjectManager characters;
    Object &add_character(const std::string &name, double x, double y, double w, double h) { return *characters.add(name, x, y, w, h); }

    ObjectManager projectiles;
    Object &add_projectile(const std::string &name, double x, double y, double w, double h) { return *projectiles.add(name, x, y, w, h); }
    button bScreen;
    SDL_Rect activeWorldCoordinates;
    SDL_Rect worldCoordinatesOnScreen;
    SDL_Rect worldCoordinatesOnMinimap;

private:
    std::unique_ptr<Map_wrapper> currentMap;
    std::unique_ptr<Map_wrapper> currentBlocks;
    // TODO: might be better to have single vector
    // and make functions which use them check
    // flags defined in the Block?
    std::vector<Block *> collisionObjects;
    std::vector<Block *> obscuringObjects;
    std::vector<Object *> damagableObjects;
    AI<Block *> ai;
    bool ai_active = false;

    std::unique_ptr<Viewport> minimap = nullptr;
};

#endif