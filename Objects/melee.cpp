#include "melee.h"

SDL_Rect Melee::get_targetZone(SDL_Rect & origin, Object::direction dir)
{
    SDL_Rect targetZone;
    switch(dir)
    {
        case Object::direction::UP:
            targetZone.x = origin.x-TILESIZEPHYSICS*(width/2);
            targetZone.y = origin.y-shift;
            targetZone.w = width*TILESIZEPHYSICS;
            targetZone.h = length*TILESIZEPHYSICS;
            break;
        case Object::direction::DOWN:
            targetZone.x = origin.x-TILESIZEPHYSICS*(width/2);
            targetZone.y = origin.y+shift;
            targetZone.w = width*TILESIZEPHYSICS;
            targetZone.h = length*TILESIZEPHYSICS;
            break;
        case Object::direction::LEFT:
            targetZone.x = origin.x-shift;
            targetZone.y = origin.y-TILESIZEPHYSICS*(width/2);
            targetZone.w = length*TILESIZEPHYSICS;
            targetZone.h = width*TILESIZEPHYSICS;
            break;
        case Object::direction::RIGHT:
            targetZone.x = origin.x+shift;
            targetZone.y = origin.y-TILESIZEPHYSICS*(width/2);
            targetZone.w = length*TILESIZEPHYSICS;
            targetZone.h = width*TILESIZEPHYSICS;
            break;
    }
    return targetZone;
}

bool Melee::evaluate_target(SDL_Rect &targetZone, SDL_Rect & origin, Object *target)
{
    if (SDL_HasIntersection(&targetZone, &target->position))
    {
        ++hits;
        target->modify_health(-damage);

        if (knockback)
        {
            double dirX = target->position.x - origin.x;
            double dirY = target->position.y - origin.y;
            target->extVelX += knockback * dirX / sqrt(dirX * dirX + dirY * dirY);
            target->extVelY += knockback * dirY / sqrt(dirX * dirX + dirY * dirY);
        }
        return true;
    }
    else return false;
}

bool Melee::evaluate(Character * ch, std::vector<Object*> targets)
{   
    // TODO: why is there wrapper here?
   // evaluate(ch->position, ch->dir, targets, wrapper);
    hits = 0;
    for(auto t : targets)
    {
        // ignore objects without health
        if(t->property.count("health")==0) continue;
        // don't attack yourself
        if(ch==t) continue;

        SDL_Rect targetZone = get_targetZone(ch->position, ch->dir);
        evaluate_target(targetZone, ch->position, t);
    }
    if(lifesteal) ch->modify_health((int) damage*lifesteal*hits);
    return true;
}

bool Melee::evaluate(SDL_Rect & origin, Object::direction dir, std::vector<Object*> targets)
{
    hits = 0;
    for(auto t : targets)
    {
        // ignore objects without health
        if(t->property.count("health")==0) continue;
        SDL_Rect targetZone = get_targetZone(origin, dir);
        evaluate_target(targetZone, origin, t);
    }
    return true;
}