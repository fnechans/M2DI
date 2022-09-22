#include "character.h"

Character::Character(uint x, uint y) : Object(x, y)
{
    intrVelX = 0;
    intrVelY = 0;

    dir = DOWN;
    speedX = 4;
    speedY = 4;

    moved = false;
}

void Character::move_left()
{
    switch(moveType)
    {
        case TopDown:
        case Sidescroll:
            intrVelX -= speedX;
            break;
    }
}

void Character::move_right()
{
    switch(moveType)
    {
        case TopDown:
        case Sidescroll:
            intrVelX += speedX;
            break;
    }
}

void Character::move_up(std::vector<Object *> &collObjects)
{
    switch(moveType)
    {
        case Sidescroll:
            if(!nextTo(position, DOWN, collObjects)) break; //only jump if standing on ground
            extVelY -= speedY;
            break;
        case TopDown:
            intrVelY -= speedY;
            break;
    }
}

void Character::move_down()
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

void Character::follow_path(std::vector<Object *> &collObjects)
{
    intrVelX = 0;
    intrVelY = 0;

    if (!path.empty())
    {
        int dirX = path.back()->position.x - position.x;
        int dirY = path.back()->position.y - position.y;

        // set speed
        if (dirX > 0) move_right();
        else if (dirX < 0) move_left();
        if (dirY > 0) move_down();
        else if (dirY < 0) move_up(collObjects);

        if (fabs(position.x - path.back()->position.x) <= TILESIZEPHYSICS && fabs(position.y - path.back()->position.y) <= TILESIZEPHYSICS && path.size() > 1)
            path.pop_back();
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
    // TODO: this whole thing needs to be done better,
    // but not sure how right now
    tools::reduce_to_zero<float>(extVelX, frictionX);
    tools::reduce_to_zero<float>(extVelY, frictionY);
    int dX = (extVelX + intrVelX)*TILESIZEPHYSICS*DELTA_T;
    int dY = (extVelY + intrVelY)*TILESIZEPHYSICS*DELTA_T;

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

        if (!doesCollide(newPosition, collObjects))
        {
            position = newPosition;
            moved = true;
            return;
        }

        tools::reduce_to_zero<int>(dX, 1);
        tools::reduce_to_zero<int>(dY, 1);
    }

    dX = (extVelX + intrVelX)*TILESIZEPHYSICS*DELTA_T;
    dY = (extVelY + intrVelY)*TILESIZEPHYSICS*DELTA_T;

    while (dX)
    {
        newPosition = position;
        newPosition.x += dX;

        if (!doesCollide(newPosition, collObjects))
        {
            position = newPosition;
            moved = true;
            return;
        }
        else set_vel0_x();

        tools::reduce_to_zero<int>(dX, 1);
    }

    dX = (extVelX + intrVelX)*TILESIZEPHYSICS*DELTA_T;
    dY = (extVelY + intrVelY)*TILESIZEPHYSICS*DELTA_T;

    while (dY)
    {
        newPosition = position;
        newPosition.y += dY;

        if (!doesCollide(newPosition, collObjects))
        {
            position = newPosition;
            moved = true;
            return;
        }
        else set_vel0_y();

        tools::reduce_to_zero<int>(dY, 1);
    }

}

bool Character::doesCollide(SDL_Rect &pos, std::vector<Object *> &collObjects)
{
    if (pos.x < 0 || pos.y < 0 || pos.x > mWidth - pos.w || pos.y > mHeight - pos.h)
        return true;

    for (auto obj : collObjects)
    {
        if (obj == this)
            continue;
        if (SDL_HasIntersection(&obj->position, &pos))
            return true;
    }
    return false;
}

bool Character::nextTo(SDL_Rect pos, direction dir, std::vector<Object *> &collObjects)
{
    switch(dir)
    {
        case UP: pos.y -= 1; break;
        case DOWN: pos.y += 1; break;
        case LEFT: pos.x -= 1; break;
        case RIGHT: pos.x += 1; break;
    }
    return doesCollide(pos, collObjects);
}
