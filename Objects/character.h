#ifndef CHARACTER_H
#define CHARACTER_H

#include "object.h"
#include "melee.h"

class Melee_instance;

class Character : public Object
{
public:
    Character(uint x = mWidth / 2, uint y = mHeight / 2);

    // Type of movement
    // TopDown - WSAD like, moves in any direction
    // Side - WS+jump
    enum MoveType {TopDown, Sidescroll};
    MoveType moveType = TopDown;
    void move_left();
    void move_right();
    void move_up(std::vector<Object *> &collObjects);
    void move_down();
    void set_movetype(MoveType type)
    {
        switch(type)
        {
            case Sidescroll:
                moveType = Sidescroll; frictionY = 0; speedY = 16; break;
            case TopDown:
                moveType = TopDown; frictionY = frictionX; speedY = speedX; break;
        }
    }

    void follow_path(std::vector<Object *> &collObjects);
    void plot_path(Window &wrapper, SDL_Rect *screen);

    std::vector<Object *> path = {};
    Object *target;

    void move(std::vector<Object *> &collObjects);
    bool doesCollide(SDL_Rect &pos, std::vector<Object *> &collObjects);
    bool nextTo(SDL_Rect pos, direction dir, std::vector<Object *> &collObjects); // here copy of pos(ition) on purpose!

    // intrinsic speed of the character
    float intrVelX;
    float intrVelY;
    direction dir;
    float speedX;
    float speedY;
    bool moved;
    std::map<std::string, Melee_instance> melees;

private:
    void set_vel0_x(){ extVelX = 0;}
    void set_vel0_y(){ extVelY = 0;}
};

#endif // CHARACTER_H