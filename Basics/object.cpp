#include "object.h"

const char *const Object::dirName[4] = {"UP", "DOWN", "LEFT", "RIGHT"};

Object::Object(uint x, uint y) : Block(x, y)
{
    extVelX = 0;
    extVelY = 0;
    frictionX = 4;
    frictionY = 4;

    intrVelX = 0;
    intrVelY = 0;

    dir = DOWN;
    speedX = 4;
    speedY = 4;

    moved = false;
}

void Object::set_health(uint value)
{
    property["health"] = value;
    property["max_health"] = value;
}

void Object::modify_health(int value)
{
    if(property.count("health") == 0) return; // skip if object does not have health
    property["health"] += value;
    if (property["health"] > property["max_health"])
        property["health"] = property["max_health"];
    if (property["health"] < 0)
        property["health"] = 0;
    if (property["health"] == 0)
        dead = true;
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

        if (std::abs(hitbox.x - path.back()->hitbox.x) <= TILESIZEPHYSICS && std::abs(hitbox.y - path.back()->hitbox.y) <= TILESIZEPHYSICS && path.size() > 1)
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

void Object::move(std::vector<Block *> &collObjects)
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

    SDL_Rect newPosition = hitbox;
    while (dX || dY)
    {
        newPosition = hitbox;
        newPosition.x += dX;
        newPosition.y += dY;

        if (!does_collide(newPosition, collObjects))
        {
            hitbox = newPosition;
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
        newPosition = hitbox;
        newPosition.x += dX;

        if (!does_collide(newPosition, collObjects))
        {
            hitbox = newPosition;
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
        newPosition = hitbox;
        newPosition.y += dY;

        if (!does_collide(newPosition, collObjects))
        {
            hitbox = newPosition;
            moved = true;
            return;
        }
        else set_vel0_y();

        tools::reduce_to_zero<int>(dY, 1);
    }

}

bool Object::does_collide(SDL_Rect &pos, std::vector<Block *> &collObjects)
{
    if (pos.x < 0 || pos.y < 0 || pos.x > (int) mWidth - pos.w || pos.y > (int) mHeight - pos.h)
        return true;

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
    switch(dir)
    {
        case UP: pos.y -= 1; break;
        case DOWN: pos.y += 1; break;
        case LEFT: pos.x -= 1; break;
        case RIGHT: pos.x += 1; break;
    }
    return does_collide(pos, collObjects);
}
