#ifndef MELEE_H
#define MELEE_H

#include "base.h"
#include "movable.h"
#include "timer.h"

#include <stdexcept>
#include <sstream>

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
        imgDMG = std::make_shared<IMG_wrapper>();
        textDMG.str("");
        textDMG << damage;
        imgDMG->load_text(textDMG.str(), {255,100,100,255});
    }
    Melee(const Melee& m) : damage(m.damage), shift(m.shift), length(m.length), width(m.width)
    {
        imgDMG = std::make_shared<IMG_wrapper>();
        textDMG.str("");
        textDMG << damage;
        imgDMG->load_text(textDMG.str(), {255,100,100,255});
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

    std::shared_ptr<IMG_wrapper> imgDMG; // shows damage dealth on screen
    std::stringstream textDMG;

    timer clock;
};

#endif // MELEE_H