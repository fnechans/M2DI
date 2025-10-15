#ifndef CHARACTER_H
#define CHARACTER_H

#include "Basics/object.h"
#include "Damage/dmgr_inst.h"

class Dmgr_instance;

class Character : public Object
{
public:
    using Object::Object;
    Character(const Character& other) : Object(other)
    {
        dmgr_insts = other.dmgr_insts;
    }
    ////////////
    // DAMAGE //
    ////////////

    std::map<std::string, Dmgr_instance> dmgr_insts;
    bool evaluate_attack(const std::string& name, std::vector<Object*>& collObjects);
    bool evaluate_attack(const std::string& name, const SDL_Rect &origin, Object::direction dir, std::vector<Object*>& collObjects);
    bool evaluate_attack(const std::string& name, const SDL_Rect &origin, const SDL_Rect& dir, std::vector<Object*>& collObjects);
private:
};

std::function<void()> l_player_move_up(Character *player);
std::function<void()> l_player_jump_up(Character *player, const std::vector<Block *>& collision_objects);
std::function<void()> l_player_move_down(Character *player);
std::function<void()> l_player_move_left(Character *player);
std::function<void()> l_player_move_right(Character *player);

#endif // CHARACTER_H