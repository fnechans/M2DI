#ifndef DAMAGER_H
#define DAMAGER_H

#include "Basics/object.h"
#include "SDL_wrapper/timer.h"

#include <stdexcept>
#include <sstream>

class Damager
{
public:
    Damager(uint _damage)
        : damage(_damage)
    {
    }
    Damager(const Damager &m) : damage(m.damage)
    {
    }
    void deal_damage(Object* target);
    void knock_back(Object* targer, const SDL_Rect& origin);

    // evaluates for all targets, and reduces health, evaluates knockback...
    // attacker is character:
    virtual bool evaluate(Object *ch, std::vector<Object *>& targets)
    {
        (void) (ch);
        (void) (targets);
        return false;
    }
    // attack at location:
    virtual bool evaluate(const SDL_Rect &origin, Object::direction dir, std::vector<Object *>& targets)
    {
        (void) (origin);
        (void) (dir);
        (void) (targets);
        return false;
    }
    // attack at location:
    virtual bool evaluate(const SDL_Rect &origin, const SDL_Rect& dir, std::vector<Object *>& targets)
    {
        (void) (origin);
        (void) (dir);
        (void) (targets);
        return false;
    }

    float lifesteal = 0; // character gains fraction of damage dealt as health
    float cooldown = 1;   //time before next attack
    float delay = 0;   //time before next attack
    uint damage; // damage done to target
    uint hits; //how many targets hit per attack, useful for e.g. lifesteal
    float knockback = 0;  // target gains velocity in direction from origin
private:
};

#endif // MELEE_H