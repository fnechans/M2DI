#include "melee.h"

SDL_Rect Melee::get_targetZone(SDL_Rect & origin, direction dir)
{
    SDL_Rect targetZone;
    switch(dir)
    {
        case direction::UP:
            targetZone.x = origin.x-TILESIZE*(width/2);
            targetZone.y = origin.y-shift;
            targetZone.w = width*TILESIZE;
            targetZone.h = length*TILESIZE;
            break;
        case direction::DOWN:
            targetZone.x = origin.x-TILESIZE*(width/2);
            targetZone.y = origin.y+shift;
            targetZone.w = width*TILESIZE;
            targetZone.h = length*TILESIZE;
            break;
        case direction::LEFT:
            targetZone.x = origin.x-shift;
            targetZone.y = origin.y-TILESIZE*(width/2);
            targetZone.w = length*TILESIZE;
            targetZone.h = width*TILESIZE;
            break;
        case direction::RIGHT:
            targetZone.x = origin.x+shift;
            targetZone.y = origin.y-TILESIZE*(width/2);
            targetZone.w = length*TILESIZE;
            targetZone.h = width*TILESIZE;
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
        target->animations["DMG"] = std::make_shared<animation>(0, -TILESIZERENDER * 2);
        target->animations["DMG"]->image = imgDMG;
        target->animations["DMG"]->clips.push_back({0, 0, imgDMG->width, imgDMG->height});
        target->animations["DMG"]->frequency = 20;
        target->animations["DMG"]->display = false;
        target->animations["DMG"]->play();

        if (knockback)
        {
            double dirX = target->position.x - origin.x;
            double dirY = target->position.y - origin.y;
            target->extVelX += knockback * dirX / sqrt(dirX * dirX + dirY * dirY);
            target->extVelY += knockback * dirY / sqrt(dirX * dirX + dirY * dirY);
        }
    }
}

void Melee::evaluate(Movable * ch, std::vector<Object*> targets, SDL_wrapper * wrapper)
{
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
}

void Melee::evaluate(SDL_Rect & origin, direction dir, std::vector<Object*> targets, SDL_wrapper * wrapper)
{
    hits = 0;
    for(auto t : targets)
    {
        // ignore objects without health
        if(t->property.count("health")==0) continue;
        SDL_Rect targetZone = get_targetZone(origin, dir);
        evaluate_target(targetZone, origin, t);
    }
}