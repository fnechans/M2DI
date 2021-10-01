#include "character.h"

Character::Character(uint x, uint y) : Object(x, y)
{
    intrVelX = 0;
    intrVelY = 0;

    dir = DOWN;
    speed = TILESIZEPHYSICS / 16;

    moved = false;
}

void Character::follow_path()
{
    tools::reduce_to_zero(intrVelX, speed);
    tools::reduce_to_zero(intrVelY, speed);

    if (!path.empty())
    {
        if (fabs(position.x - path.back()->position.x) <= TILESIZEPHYSICS && fabs(position.y - path.back()->position.y) <= TILESIZEPHYSICS && path.size() > 1)
            path.pop_back();
        int dirX = path.back()->position.x - position.x;
        int dirY = path.back()->position.y - position.y;

        // set speed
        if (dirX > 0)
            intrVelX = speed;
        else if (dirX < 0)
            intrVelX = -speed;
        if (dirY > 0)
            intrVelY = speed;
        else if (dirY < 0)
            intrVelY = -speed;
        /*
        // set dir
        if( fabs(dirX) > fabs(dirY) ) 
        {
            if( dirX > 0 ) dir = RIGHT;
            else if( dirX < 0 ) dir = LEFT;
        }
        else
        {
            if( dirY > 0 ) dir = DOWN;
            else if( dirY < 0 ) dir = UP;
        }*/
    }
}

void Character::plot_path(Window &wrapper, SDL_Rect *screen)
{
    if (!path.empty())
    {
        for (auto t : path)
        {
            t->image->set_color(mapColor.r, mapColor.g, mapColor.b);
            t->plot(wrapper, screen);
            t->image->set_color(255, 255, 255);
        }
    }
}

void Character::move(std::vector<Object *> &collObjects)
{
    // TODO: this needs to be done better, but not sure how right now
    tools::reduce_to_zero(extVelX, friction);
    tools::reduce_to_zero(extVelY, friction);
    int dX = extVelX + intrVelX;
    int dY = extVelY + intrVelY;

    moved = false;
    if (intrVelX > 0)
        dir = RIGHT;
    else if (intrVelX < 0)
        dir = LEFT;
    else if (intrVelY > 0)
        dir = DOWN;
    else if (intrVelY < 0)
        dir = UP;
    if (dX == 0 && dY == 0)
        return;

    SDL_Rect newPosition = position;
    while (dX || dY)
    {
        newPosition = position;
        newPosition.x += dX;
        newPosition.y += dY;

        if (newPosition.x < 0)
            newPosition.x = 0;
        if (newPosition.y < 0)
            newPosition.y = 0;
        if (newPosition.x > mWidth - newPosition.w)
            newPosition.x = mWidth - newPosition.w;
        if (newPosition.y > mHeight - newPosition.h)
            newPosition.y = mHeight - newPosition.h;

        if (!doesCollide(newPosition, collObjects))
        {
            position = newPosition;
            moved = true;
            return;
        }

        tools::reduce_to_zero(dX, 1);
        tools::reduce_to_zero(dY, 1);
    }

    dX = extVelX + intrVelX;
    dY = extVelY + intrVelY;

    while (dX)
    {
        newPosition = position;
        newPosition.x += dX;

        if (newPosition.x < 0)
            newPosition.x = 0;
        if (newPosition.y < 0)
            newPosition.y = 0;
        if (newPosition.x > mWidth - newPosition.w)
            newPosition.x = mWidth - newPosition.w;
        if (newPosition.y > mHeight - newPosition.h)
            newPosition.y = mHeight - newPosition.h;

        if (!doesCollide(newPosition, collObjects))
        {
            position = newPosition;
            moved = true;
            return;
        }

        tools::reduce_to_zero(dX, 1);
    }

    dX = extVelX + intrVelX;
    dY = extVelY + intrVelY;

    while (dY)
    {
        newPosition = position;
        newPosition.y += dY;

        if (newPosition.x < 0)
            newPosition.x = 0;
        if (newPosition.y < 0)
            newPosition.y = 0;
        if (newPosition.x > mWidth - newPosition.w)
            newPosition.x = mWidth - newPosition.w;
        if (newPosition.y > mHeight - newPosition.h)
            newPosition.y = mHeight - newPosition.h;

        if (!doesCollide(newPosition, collObjects))
        {
            position = newPosition;
            moved = true;
            return;
        }

        tools::reduce_to_zero(dY, 1);
    }
}

bool Character::doesCollide(SDL_Rect &pos, std::vector<Object *> &collObjects)
{

    for (auto obj : collObjects)
    {
        if (obj == this)
            continue;
        if (SDL_HasIntersection(&obj->position, &pos))
            return true;
    }
    return false;
}
