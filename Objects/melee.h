#ifndef MELEE_H
#define MELEE_H

#include "base.h"
#include "movable.h"
#include "timer.h"

#include <stdexcept>
#include <sstream>

// TODO: Could this be avoided?
class Movable;

class Melee : public base
{
public:
    Melee(uint _damage, int _shift = 1, uint _length = 1, uint _width = 3)
    : damage(_damage), shift(_shift), length(_length), width(_width)
    {
        if(width%2 == 0)
        {
            throw std::invalid_argument("Attack width has to be odd!");
        }
    }
    Melee(const Melee& m) : damage(m.damage), shift(m.shift), length(m.length), width(m.width)
    {
    }
    // evaluates if target was hit, returns true if yes
    bool evaluate_target(SDL_Rect & targetZone, SDL_Rect & origin, Object * target);
    // evaluates for all targets, and reduces health, evaluates knockback...
    // attacker is character:
    bool evaluate(Movable * ch, std::vector<Object*> targets);
    // attack at location:
    bool evaluate(SDL_Rect & origin, Object::direction dir, std::vector<Object*> targets);
    
    // TODO: move private, helper function
    uint knockback = 0; // target gains velocity in direction from origin
    float lifesteal = 0; // character gains fraction of damage dealt as health
    uint cooldown = 1; //time before next attack
private:
    SDL_Rect get_targetZone(SDL_Rect & origin, Object::direction dir);

    // properties
    uint damage; // damage done to target
    int shift; // shift of target rect in direction of attack
    uint length; // length of attack
    uint width; // width of attack
               // 0 is on top of origin, 1 right in front of it
    
    // internal counters
    uint hits; //how many targets hit per attack, useful for e.g. lifesteal
};


// Melee class is general and holds information about attack we want
// in the game game
// Melee_instance is specific for a character, mainly to handle cooldown
// (and probably other things) and other properties dependent on the actor.
// Since it only points to melee it is much smaller
class Melee_instance
{
public:
    Melee_instance(Melee* _melee){melee = _melee;}
    Melee_instance(const Melee_instance& m){melee = m.melee;}

    // simply wrap around the evaluate functions from
    // the Melee
    bool evaluate(Movable * ch, std::vector<Object*> targets)
    {
        if(clock.isStarted && clock.getTicks()<melee->cooldown*1000)
            return false;
        else clock.restart();
        return melee->evaluate(ch, targets);
    }
    bool evaluate(SDL_Rect & origin, Object::direction dir, std::vector<Object*> targets)
    {
        if(clock.isStarted && clock.getTicks()<melee->cooldown*1000)
            return false;
        else clock.restart();
        return melee->evaluate(origin, dir, targets);
    }
private:
    Melee* melee;
    timer clock;
};

#endif // MELEE_H