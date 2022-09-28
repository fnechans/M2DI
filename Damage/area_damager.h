#ifndef AREADMGR_H
#define AREADMGR_H

#include "damager.h"

#include <stdexcept>
#include <sstream>


class AreaDamager : public Damager
{
public:
    AreaDamager(uint _damage, float _shift = 0.5, float _length = 1, float _width = 3)
        : Damager(_damage), shift(_shift), length(_length), width(_width)
    {
    }
    AreaDamager(const AreaDamager &m) : Damager(m.damage), shift(m.shift), length(m.length), width(m.width)
    {
    }

    // evaluates if target was hit, returns true if yes
    bool evaluate_target(SDL_Rect &targetZone, const SDL_Rect &origin, Object *target);
    // evaluates for all targets, and reduces health, evaluates knockback...
    // attacker is character:
    virtual bool evaluate(Object *ch, std::vector<Object *>& targets) override;
    // attack at location:
    virtual bool evaluate(const SDL_Rect &origin, Object::direction dir, std::vector<Object *>& targets) override;

private:
    SDL_Rect get_targetZone(const SDL_Rect &origin, Object::direction dir);

    // properties
    float shift;   // shift of target rect in direction of attack
    float length; // length of attack
    float width;  // width of attack
                 // 0 is on top of origin, 1 right in front of it
};


#endif // AREADMGR_H