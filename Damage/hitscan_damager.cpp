#include "hitscan_damager.h"


bool HitScanDamager::evaluate_target(Object *target)
{
    const SDL_Rect& hb = target->hitbox;
    bool hasIntersect =
        tools::line_intersect(origin, endpoint, {hb.x, hb.y, 0,0}, {hb.x+hb.w, hb.y, 0,0})
        || tools::line_intersect(origin, endpoint, {hb.x, hb.y, 0,0}, {hb.x, hb.y+hb.h, 0,0})
        || tools::line_intersect(origin, endpoint, {hb.x+hb.w, hb.y+hb.h, 0,0}, {hb.x+hb.w, hb.y, 0,0})
        || tools::line_intersect(origin, endpoint, {hb.x+hb.w, hb.y+hb.h, 0,0}, {hb.x, hb.y+hb.h, 0,0});

    if(hasIntersect)
    {
        deal_damage(target);

        if (knockback) knock_back(target, origin);

        return true;
    }
    else
        return false;
}

bool HitScanDamager::evaluate(const SDL_Rect &_origin, const SDL_Rect& dir, std::vector<Object *>& targets)
{
    endpoint = tools::get_endpoint(_origin, dir, range*base::TILESIZEPHYSICS);
    origin = tools::get_endpoint(endpoint, _origin, (range-shift)*base::TILESIZEPHYSICS);
    hits = 0;
    for (auto t : targets)
    {
        // quick hack to avoid self-damage
        if(SDL_HasIntersection(&_origin, &t->hitbox) ) continue;
        // ignore objects without health
        if (t->dead)
            continue;
        evaluate_target(t);
    }
    return true;
}