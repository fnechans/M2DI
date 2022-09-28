#include "hitscan_damager.h"

// helper functions"
SDL_Rect get_endpoint(const SDL_Rect& origin, const SDL_Rect &direction, int range)
{
    int dX = direction.x - origin.x;
    int dY = direction.y - origin.y;
    float dR = sqrt(dX*dX + dY*dY);
    return {origin.x + range * dX / dR, origin.y + range * dY / dR, 0, 0};
}

bool line_intersect(const SDL_Rect &X1, const SDL_Rect &X2,
                    const SDL_Rect &Y1, const SDL_Rect &Y2)
{
    long denom = (X1.x - X2.x) * (Y1.y - Y2.y) - (X1.y - X2.y) * (Y1.x - Y2.x);
    // if denom 0 then the lines are paralel:
    if (denom == 0)
        return false;

    if (denom > 0)
    {
        long num1 = (X1.x - Y1.x) * (Y1.y - Y2.y) - (X1.y - Y1.y) * (Y1.x - Y2.x);
        if (num1 < 0 || num1 > denom)
            return false;

        long num2 = (X1.x - Y1.x) * (X1.y - X2.y) - (X1.y - Y1.y) * (X1.x - X2.x);
        if (num2 < 0 || num2 > denom)
            return false;

    }
    else // denom < 0
    {
        long num1 = (X1.x - Y1.x) * (Y1.y - Y2.y) - (X1.y - Y1.y) * (Y1.x - Y2.x);
        if (num1 > 0 || num1 < denom)
            return false;

        long num2 = (X1.x - Y1.x) * (X1.y - X2.y) - (X1.y - Y1.y) * (X1.x - X2.x);
        if (num2 > 0 || num2 < denom)
            return false;

    }
    return true;
}

bool HitScanDamager::evaluate_target(Object *target)
{
    const SDL_Rect& hb = target->hitbox;
    bool hasIntersect =
        line_intersect(origin, endpoint, {hb.x, hb.y, 0,0}, {hb.x+hb.w, hb.y, 0,0})
        || line_intersect(origin, endpoint, {hb.x, hb.y, 0,0}, {hb.x, hb.y+hb.h, 0,0})
        || line_intersect(origin, endpoint, {hb.x+hb.w, hb.y+hb.h, 0,0}, {hb.x+hb.w, hb.y, 0,0})
        || line_intersect(origin, endpoint, {hb.x+hb.w, hb.y+hb.h, 0,0}, {hb.x, hb.y+hb.h, 0,0});

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
    endpoint = get_endpoint(_origin, dir, range*base::TILESIZEPHYSICS);
    origin = get_endpoint(endpoint, _origin, (range-shift)*base::TILESIZEPHYSICS);

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