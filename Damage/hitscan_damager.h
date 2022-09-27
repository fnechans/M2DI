#ifndef HITSCAN_DMGR_H
#define HITSCAN_DMGR_H

/*#include "damager.h"

#include <stdexcept>
#include <sstream>


class HitScanDamager : public Damager
{
public:
    HitScanDamager(uint _damage, float _range)
        : Damager(_damage), range(_range)
    {
    }
    HitScanDamager(const HitScanDamager &m) : Damager(m.damage), range(m.range)
    {
    }
    // evaluates if target was hit, returns true if yes
    bool evaluate_target(SDL_Rect &targetZone, const SDL_Rect &origin, Object *target);
    // evaluates for all targets, and reduces health, evaluates knockback...
    // attacker is character:
    bool evaluate(Character *ch, std::vector<Object *>& targets);
    // attack at location:
    bool evaluate(const SDL_Rect &origin, Object::direction dir, std::vector<Object *>& targets);
    bool evaluate(Character *ch, SDL_Rect dir_point, std::vector<Object *>& targets);
    // attack at location:
    bool evaluate(const SDL_Rect &origin, SDL_Rect dir_point, std::vector<Object *>& targets);
    uint knockback = 0;  // target gains velocity in direction from origin
private:
    SDL_Rect get_targetZone(const SDL_Rect &origin, Object::direction dir);

    // properties
    float range;   // shift of target rect in direction of attack
};
*/
#endif