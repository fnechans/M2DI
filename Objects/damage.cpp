#include "damage.h"

SDL_Rect Damager::get_targetZone(const SDL_Rect &origin, Object::direction dir)
{
    SDL_Rect targetZone;
    switch (dir)
    {
    case Object::direction::UP:
        targetZone.x = origin.x - (width/2) * TILESIZEPHYSICS;
        targetZone.y = origin.y - (shift + length/2) * TILESIZEPHYSICS;
        targetZone.w = width * TILESIZEPHYSICS;
        targetZone.h = length * TILESIZEPHYSICS;
        break;
    case Object::direction::DOWN:
        targetZone.x = origin.x - (width/2) * TILESIZEPHYSICS;
        targetZone.y = origin.y + (shift - length/2) * TILESIZEPHYSICS;
        targetZone.w = width * TILESIZEPHYSICS;
        targetZone.h = length * TILESIZEPHYSICS;
        break;
    case Object::direction::LEFT:
        targetZone.x = origin.x - (shift + length/2) * TILESIZEPHYSICS;
        targetZone.y = origin.y - (width/2) * TILESIZEPHYSICS;
        targetZone.w = length * TILESIZEPHYSICS;
        targetZone.h = width * TILESIZEPHYSICS;
        break;
    case Object::direction::RIGHT:
        targetZone.x = origin.x + (shift - length/2) * TILESIZEPHYSICS;
        targetZone.y = origin.y - (width/2) * TILESIZEPHYSICS;
        targetZone.w = length * TILESIZEPHYSICS;
        targetZone.h = width * TILESIZEPHYSICS;
        break;
    }
    return targetZone;
}

bool Damager::evaluate_target(SDL_Rect &targetZone, const SDL_Rect &origin, Object *target)
{
    if (SDL_HasIntersection(&targetZone, &target->hitbox))
    {
        ++hits;
        target->modify_health(-damage);

        if (knockback)
        {
            double dirX = target->hitbox.x - origin.x;
            double dirY = target->hitbox.y - origin.y;
            target->extVelX += knockback * dirX / sqrt(dirX * dirX + dirY * dirY);
            target->extVelY += knockback * dirY / sqrt(dirX * dirX + dirY * dirY);
        }
        return true;
    }
    else
        return false;
}

bool Damager::evaluate(Character *ch, std::vector<Object *>& targets)
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

bool Damager::evaluate(const SDL_Rect &origin, Object::direction dir, std::vector<Object *>& targets)
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