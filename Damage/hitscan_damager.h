#ifndef HITSCAN_DMGR_H
#define HITSCAN_DMGR_H

#include "damager.h"

#include <stdexcept>
#include <sstream>


class HitScanDamager : public Damager
{
public:
    HitScanDamager(uint _damage, float _range, float _shift=0)
        : Damager(_damage), range(_range), shift(_shift)
    {
    }
    HitScanDamager(const HitScanDamager &m) : Damager(m.damage), range(m.range)
    {
    }

    // evaluates if target was hit, returns true if yes
    bool evaluate_target(Object *target);
    // attack at location:
    virtual bool evaluate(const SDL_Rect &_origin, const SDL_Rect& direction, std::vector<Object *>& targets) override;

    // properties
    float range;   // shift of target rect in direction of attack
    float shift;   // shift of target rect in direction of attack
    SDL_Rect origin;
    SDL_Rect endpoint;
private:
};

#endif