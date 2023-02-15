#include "object.h"


Object::Object(const Object& other) : Block(other)
{
    moveType = other.moveType;
    speedX = other.speedX;
    speedY = other.speedY;
    bounceFactor = other.bounceFactor;
    doPlotPath = other.doPlotPath;
    target = other.target;
}

void Object::set_health(uint value)
{
    property["health"] = value;
    property["max_health"] = value;
}

void Object::modify_health(int value)
{
    if(property.count("health") == 0)
    {
        std::cout << "Attemping to modify health but non health propery\n";
        return; // skip if object does not have health
    }
    property["health"] += value;
    if (property["health"] > property["max_health"])
        property["health"] = property["max_health"];
    if (property["health"] < 0)
        property["health"] = 0;
    if (property["health"] == 0)
    {
        dead = true;
    }
}

void Object::move_left()
{
    switch(moveType)
    {
        case TopDown:
        case Sidescroll:
            intrVelX -= speedX;
            break;
    }
}

void Object::move_right()
{
    switch(moveType)
    {
        case TopDown:
        case Sidescroll:
            intrVelX += speedX;
            break;
    }
}

void Object::move_up(std::vector<Block *> &collObjects)
{
    switch(moveType)
    {
        case Sidescroll:
            if(!next_to(hitbox, DOWN, collObjects)) break; //only jump if standing on ground
            extVelY -= speedY;
            break;
        case TopDown:
            intrVelY -= speedY;
            break;
    }
}

void Object::move_down()
{
    switch(moveType)
    {
        case Sidescroll:
            break;
        case TopDown:
            intrVelY += speedY;
            break;
    }
}

void Object::follow_path(std::vector<Block *> &collObjects)
{
    intrVelX = 0;
    intrVelY = 0;

    if (!path.empty())
    {
        int dirX = path.back()->hitbox.x - hitbox.x;
        int dirY = path.back()->hitbox.y - hitbox.y;

        // set speed
        if (dirX > 0) move_right();
        else if (dirX < 0) move_left();
        if (dirY > 0) move_down();
        else if (dirY < 0) move_up(collObjects);

        if ((uint) std::abs(hitbox.x - path.back()->hitbox.x) <= base::TILESIZEPHYSICS &&
            (uint) std::abs(hitbox.y - path.back()->hitbox.y) <= base::TILESIZEPHYSICS && path.size() > 1)
            path.pop_back();
    }
}

void Object::plot_path(Window &wrapper, SDL_Rect *screen)
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

void Object::move(std::vector<Block *> &collObjects, double DELTA_T)
{
    // TODO: this whole thing needs to be done better,
    // but not sure how right now
    tools::reduce_to_zero<float>(extVelX, frictionX*DELTA_T);
    tools::reduce_to_zero<float>(extVelY, frictionY*DELTA_T);
    int dX0 = (extVelX + intrVelX)*base::TILESIZEPHYSICS*DELTA_T;
    int dY0 = (extVelY + intrVelY)*base::TILESIZEPHYSICS*DELTA_T;

    moved = false;
    bounced = false;
    if (intrVelX > 0)
        dir = RIGHT;
    else if (intrVelX < 0)
        dir = LEFT;
    else if (intrVelY > 0)
        dir = DOWN;
    else if (intrVelY < 0)
        dir = UP;

    int dX = dX0;
    int dY = dY0;
    while (dX || dY)
    {
        // First check if we can move full distance
        SDL_Rect newPosition = hitbox;
        newPosition.x += dX;
        newPosition.y += dY;
        if (!does_collide(newPosition, collObjects))
        {
            hitbox = newPosition;
            moved = intrVelX || intrVelY;
            return;
        }

        // Can we move by reducing distance in one direction?
        int dX2 = dX;
        int dY2 = dY;
        bool successX = false;
        bool successY = false;
        while (dX2)
        {
            tools::reduce_to_zero<int>(dX2, 1);
            newPosition = hitbox;
            newPosition.x += dX2;
            newPosition.y += dY;

            if (!does_collide(newPosition, collObjects))
            {
                successX = true;
                break;
            }
        }

        while (dY2)
        {
            tools::reduce_to_zero<int>(dY2, 1);
            newPosition = hitbox;
            newPosition.x += dX;
            newPosition.y += dY2;

            if (!does_collide(newPosition, collObjects))
            {
                successY = true;
                break;
            }
        }
        // If neither succeeded, reduce both original coor. by one
        // and repeat
        if(!successX && !successY)
        {
            tools::reduce_to_zero<int>(dX, 1);
            tools::reduce_to_zero<int>(dY, 1);
        }

        // Choose version which changes the movement the least
        // or the one that succeeded
        // and perform bounce in the direction of the reduction
        // (bounce of an obstacle)
        if (successY && (!successX || std::abs(dY2 - dY) < std::abs(dX2 - dX)))
        {
            hitbox.x += dX;
            hitbox.y += dY2;
            moved = intrVelX || intrVelY;
            bounced = true;
            extVelX = bounceFactor * extVelX;
            extVelY = -bounceFactor * extVelY;
            return;
        }
        if (successX && (!successY || std::abs(dY2 - dY) >= std::abs(dX2 - dX)))
        {
            hitbox.x += dX2;
            hitbox.y += dY;
            moved = intrVelX || intrVelY;
            bounced = true;
            extVelX = -bounceFactor * extVelX;
            extVelY = bounceFactor * extVelY;
            return;
        }
    }

    // If nothing succeeded, it probably means object is stuck
    // in another object. This can happen in interaction between
    // two moving objects where one does not collide with the other
    // (e.g. grenade with char, where we dont want granades to
    // obstruct chars but want them to bounce of chars)
    // For now, kill their velocity, but in future might be better
    // to kick them away? Would be complicated but doesCollide
    // could in theory return pointer, then one could just
    // move away from CMS with appropriate extVel?
    extVelX = 0;
    extVelY = 0;
}

bool Object::does_collide(SDL_Rect &pos, std::vector<Block *> &collObjects)
{

    for (auto obj : collObjects)
    {
        if (obj == this)
            continue;
        if (SDL_HasIntersection(&obj->hitbox, &pos))
            return true;
    }
    return false;
}

bool Object::next_to(SDL_Rect pos, direction dir, std::vector<Block *> &collObjects)
{
    switch (dir)
    {
    case UP:
        pos.y -= 1;
        break;
    case DOWN:
        pos.y += 1;
        break;
    case LEFT:
        pos.x -= 1;
        break;
    case RIGHT:
        pos.x += 1;
        break;
    }

    return does_collide(pos, collObjects);
}

void Object::kick(const SDL_Rect &origin, float multiplier)
{
    double dirX = position().x - origin.x;
    double dirY = position().y - origin.y;
    extVelX += multiplier * dirX / sqrt(dirX * dirX + dirY * dirY);
    extVelY += multiplier * dirY / sqrt(dirX * dirX + dirY * dirY);
}