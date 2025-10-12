#include "character.h"

bool Character::evaluate_attack(const std::string& name, std::vector<Object*>& targets)
{
    if(dmgr_insts.count(name) == 0) return false;
    auto & dmgr_inst = dmgr_insts.at(name);
     return dmgr_inst.doAtack
         && dmgr_inst.check_cooldown()
         && dmgr_inst.dmgr->evaluate(this, targets);
}

bool Character::evaluate_attack(const std::string& name, const SDL_Rect &origin, Object::direction dir, std::vector<Object*>& targets)
 {
    if(dmgr_insts.count(name) == 0) return false;
    auto & dmgr_inst = dmgr_insts.at(name);
     return dmgr_inst.doAtack
         && dmgr_inst.check_cooldown()
         && dmgr_inst.dmgr->evaluate(origin, dir, targets);
 }

 bool Character::evaluate_attack(const std::string& name, const SDL_Rect &origin, const SDL_Rect& dir, std::vector<Object*>& targets)
 {
    if(dmgr_insts.count(name) == 0) return false;
    auto & dmgr_inst = dmgr_insts.at(name);
     return dmgr_inst.doAtack
         && dmgr_inst.check_cooldown()
         && dmgr_inst.dmgr->evaluate(origin, dir, targets);
 }


 std::function<void()> l_player_move_up(Character *player)
{
    return [player]()
    { player->move_up({}); };
}

std::function<void()> l_player_jump_up(Character *player, const std::vector<Block *>& collision_objects)
{
    return [player, &collision_objects = std::as_const(collision_objects)]()
    { player->move_up(collision_objects); };
}

std::function<void()> l_player_move_down(Character *player)
{
    return [player]()
    { player->move_down(); };
}

std::function<void()> l_player_move_left(Character *player)
{
    return [player]()
    { player->move_left(); };
}

std::function<void()> l_player_move_right(Character *player)
{
    return [player]()
    { player->move_right(); };
}