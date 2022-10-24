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