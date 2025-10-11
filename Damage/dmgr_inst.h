#ifndef DMGR_INST_H
#define DMGR_INST_H

#include "SDL_wrapper/timer.h"
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
    Dmgr_instance(Damager *_melee)
    {
        dmgr = _melee;
        if(dmgr->delay) delay_clock.start();
    }
    Dmgr_instance(const Dmgr_instance &m)
    {
        dmgr = m.dmgr;
        if(dmgr->delay) delay_clock.start();
    }

    bool check_cooldown(bool restart=true)
    {
        if (delay_clock.isStarted && delay_clock.getTicks() < dmgr->delay * 1000)
            return false;
        delay_clock.stop();
        if (cd_clock.isStarted && cd_clock.getTicks() < dmgr->cooldown * 1000)
            return false;
        else if(restart)
            cd_clock.restart();
        return true;
    }
    // e.g. for display of cooldown
    float get_cooldown_fraction()
    {
        if (cd_clock.isStarted && cd_clock.getTicks() < dmgr->cooldown*1000)
            return cd_clock.getTicks()/(dmgr->cooldown*1000);
        return 1;
    }

    Damager *dmgr;
    bool doAtack = false;
protected:
    Timer cd_clock;
    Timer delay_clock;
};


#endif // MELEE_H