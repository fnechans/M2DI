#ifndef DMGR_INST_H
#define DMGR_INST_H

#include "timer.h"
#include "damager.h"
#include "area_damager.h"
#include "hitscan_damager.h"

#include <stdexcept>
#include <sstream>

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

    bool check_cooldown(bool restart=true)
    {
        if (clock.isStarted && clock.getTicks() < dmgr->cooldown * 1000)
            return false;
        else if(restart)
            clock.restart();
        return true;
    }
    // e.g. for display of cooldown
    float get_cooldown_fraction()
    {
        if (clock.isStarted && clock.getTicks() < dmgr->cooldown*1000)
            return clock.getTicks()/(dmgr->cooldown*1000);
        return 1;
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