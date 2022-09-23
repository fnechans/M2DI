#ifndef MELEE_H
#define MELEE_H

#include "base.h"
#include "character.h"
#include "timer.h"

#include <stdexcept>
#include <sstream>

// TODO: Could this be avoided?
class Character;

class Damager : public base
{
public:
    Damager(uint _damage, float _shift = 0.5, float _length = 1, float _width = 3)
        : damage(_damage), shift(_shift), length(_length), width(_width)
    {
    }
    Damager(const Damager &m) : damage(m.damage), shift(m.shift), length(m.length), width(m.width)
    {
    }
    // evaluates if target was hit, returns true if yes
    bool evaluate_target(SDL_Rect &targetZone, const SDL_Rect &origin, Object *target);
    // evaluates for all targets, and reduces health, evaluates knockback...
    // attacker is character:
    bool evaluate(Character *ch, std::vector<Object *>& targets);
    // attack at location:
    bool evaluate(const SDL_Rect &origin, Object::direction dir, std::vector<Object *>& targets);

    // TODO: move private, helper function
    uint knockback = 0;  // target gains velocity in direction from origin
    float lifesteal = 0; // character gains fraction of damage dealt as health
    uint cooldown = 1;   //time before next attack
private:
    SDL_Rect get_targetZone(const SDL_Rect &origin, Object::direction dir);

    // properties
    uint damage; // damage done to target
    float shift;   // shift of target rect in direction of attack
    float length; // length of attack
    float width;  // width of attack
                 // 0 is on top of origin, 1 right in front of it

    // internal counters
    uint hits; //how many targets hit per attack, useful for e.g. lifesteal
};

// Damager class is general and holds information about attack type we want
// in the game game
// Dmgr_instance is specific for a character, mainly to handle cooldown
// (and probably other things) and other properties dependent on the actor.
// Since it only points to dmgr it is much smaller
class Dmgr_instance
{
public:
    Dmgr_instance(Damager *_melee) { dmgr = _melee; }
    Dmgr_instance(const Dmgr_instance &m) { dmgr = m.dmgr; }

    // simply wrap around the evaluate functions from
    // the Melee
    bool evaluate(Character *ch, std::vector<Object *>& targets)
    {
        if (clock.isStarted && clock.getTicks() < dmgr->cooldown * 1000)
            return false;
        else
            clock.restart();
        return dmgr->evaluate(ch, targets);
    }
    bool evaluate(const SDL_Rect &origin, Object::direction dir, std::vector<Object *>& targets)
    {
        if (clock.isStarted && clock.getTicks() < dmgr->cooldown * 1000)
            return false;
        else
            clock.restart();
        return dmgr->evaluate(origin, dir, targets);
    }

    Damager *dmgr;
    bool doAtack = false;
protected:
    timer clock;
};

//class  : public Dmgr_instance
//{
//public:
//    Melee(Damager *_melee) : Dmgr_instance(_melee) {}
//    Melee(const Dmgr_instance &m) : Dmgr_instance(m) {}
//};
#endif // MELEE_H