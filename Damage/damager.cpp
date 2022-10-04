#include "damager.h"

void Damager::deal_damage(Object* target)
{
    ++hits;
    target->modify_health(-damage);
}

void Damager::knock_back(Object *target, const SDL_Rect& origin)
{
        double dirX = target->position().x - origin.x;
        double dirY = target->position().y - origin.y;
        target->extVelX += knockback * dirX / sqrt(dirX * dirX + dirY * dirY);
        target->extVelY += knockback * dirY / sqrt(dirX * dirX + dirY * dirY);
}