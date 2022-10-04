#ifndef OBJECT_H
#define OBJECT_H

#include "block.h"

class Object : public Block
{
public:
    Object(uint x = mWidth / 2, uint y = mHeight / 2);

    void set_health(uint value);
    void modify_health(int value);
    bool dead = false;

    /////////////
    // MOVEMENT//
    /////////////

    enum direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    static const char *const dirName[4]; // = {"UP", "DOWN", "LEFT", "RIGHT"};

    // Type of movement
    // TopDown - WSAD like, moves in any direction
    // Side - WS+jump
    enum MoveType {TopDown, Sidescroll};
    MoveType moveType = TopDown;
    void move_left();
    void move_right();
    void move_up(std::vector<Block *> &collObjects);
    void move_down();

    void kick(float dax, float day){ extVelX += dax; extVelY += day; }

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

    void follow_path(std::vector<Block *>& collObjects);
    void plot_path(Window &wrapper, SDL_Rect *screen);

    std::vector<Block *> path = {};
    Object *target{nullptr};

    void move(std::vector<Block *> &collObjects);
    bool does_collide(SDL_Rect &pos, std::vector<Block *>& collObjects);
    bool next_to(SDL_Rect pos, direction dir, std::vector<Block *>& collObjects); // here copy of pos(ition) on purpose!

    // intrinsic speed of the character
    float intrVelX;
    float intrVelY;
    float speedX;
    float speedY;
    bool moved;
    // velocity based of external factors (knockback)
    float extVelX;
    float extVelY;
    // friction impacts extVel
    float frictionX;
    float frictionY;
    direction dir;

private:
    void set_vel0_x(){ extVelX = 0;}
    void set_vel0_y(){ extVelY = 0;}
};

#endif // CHARACTER_H