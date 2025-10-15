#include "area_damager.h"

SDL_Rect AreaDamager::get_targetZone(const SDL_Rect &origin, int dir)
{
    SDL_Rect targetZone;
    switch (dir)
    {
    case Object::direction::UP:
        targetZone.x = origin.x - (width/2) * base::TILESIZEPHYSICS;
        targetZone.y = origin.y - (shift + length/2) * base::TILESIZEPHYSICS;
        targetZone.w = width * base::TILESIZEPHYSICS;
        targetZone.h = length * base::TILESIZEPHYSICS;
        break;
    case Object::direction::DOWN:
        targetZone.x = origin.x - (width/2) * base::TILESIZEPHYSICS;
        targetZone.y = origin.y + (shift - length/2) * base::TILESIZEPHYSICS;
        targetZone.w = width * base::TILESIZEPHYSICS;
        targetZone.h = length * base::TILESIZEPHYSICS;
        break;
    case Object::direction::LEFT:
        targetZone.x = origin.x - (shift + length/2) * base::TILESIZEPHYSICS;
        targetZone.y = origin.y - (width/2) * base::TILESIZEPHYSICS;
        targetZone.w = length * base::TILESIZEPHYSICS;
        targetZone.h = width * base::TILESIZEPHYSICS;
        break;
    case Object::direction::RIGHT:
        targetZone.x = origin.x + (shift - length/2) * base::TILESIZEPHYSICS;
        targetZone.y = origin.y - (width/2) * base::TILESIZEPHYSICS;
        targetZone.w = length * base::TILESIZEPHYSICS;
        targetZone.h = width * base::TILESIZEPHYSICS;
        break;
    }
    return targetZone;
}

bool AreaDamager::evaluate_target(SDL_Rect &targetZone, const SDL_Rect &origin, Object *target)
{
    if (SDL_HasIntersection(&targetZone, &target->hitbox))
    {
        deal_damage(target);

        if (knockback) knock_back(target, origin);

        return true;
    }
    else
        return false;
}

bool AreaDamager::evaluate(Object *ch, std::vector<Object *>& targets)
{
    // TODO: why is there wrapper here?
    // evaluate(ch->position, ch->dir, targets, wrapper);
    hits = 0;
    SDL_Rect targetZone = get_targetZone(ch->position(), ch->dir);
    for (auto t : targets)
    {
        // ignore objects without health
        if (t->dead)
            continue;
        // don't attack yourself
        if (ch == t)
            continue;

        evaluate_target(targetZone, ch->hitbox, t);
    }
    if (lifesteal)
        ch->modify_health((int)damage * lifesteal * hits);
    return true;
}

bool AreaDamager::evaluate(const SDL_Rect &origin, Object::direction dir, std::vector<Object *>& targets)
{
    hits = 0;
    SDL_Rect targetZone = get_targetZone(origin, dir);
    for (auto t : targets)
    {
        // ignore objects without health
        if (t->dead)
            continue;
        evaluate_target(targetZone, origin, t);
    }
    return true;
}