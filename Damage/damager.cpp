#include "damager.h"

void Damager::deal_damage(Object* target)
{
    ++hits;
    target->modify_health(-damage);
}

void Damager::knock_back(Object *target, const SDL_Rect& origin)
{
    target->kick(origin, knockback);
}