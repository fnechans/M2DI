#ifndef OBJECT_H
#define OBJECT_H

#include "block.h"

class Object : public Block
{
public:
    using Block::Block;

    Object(const Object& other);

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
    static inline const char *const dirName[4]{"UP", "DOWN", "LEFT", "RIGHT"};


    // Type of movement
    // TopDown - WSAD like, moves in any direction
    // Side - WS+jump
    enum MoveType {TopDown, Sidescroll};
    MoveType moveType = TopDown;
    void move_left();
    void move_right();
    void move_up(std::vector<Block *> &collObjects);
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

    void follow_path(std::vector<Block *>& collObjects);
    void plot_path(Window &wrapper, SDL_Rect *screen);

    std::vector<Block *> path = {};
    Object *target{nullptr};

    void move(std::vector<Block *> &collObjects, double DELTA_T);
    bool does_collide(SDL_Rect &pos, std::vector<Block *>& collObjects);
    bool next_to(SDL_Rect pos, direction dir, std::vector<Block *>& collObjects); // here copy of pos(ition) on purpose!

    // intrinsic speed of the character
    float intrVelX{0.};
    float intrVelY{0.};
    float speedX{4.};
    float speedY{4.};
    bool moved{false};
    // velocity based of external factors (knockback)
    float extVelX{0.};
    float extVelY{0.};
    // friction impacts extVel
    float frictionX{4.};
    float frictionY{4.};
    float bounceFactor = 0; // fraction of velocity object gets back when colliding with wall
    bool bounced;
    direction dir{DOWN};
    bool doPlotPath{false};
    std::string dir_name(){ return dirName[dir]; }

    void kick(const SDL_Rect& dir, float multiplier);
    void kick(float dax, float day){ extVelX += dax; extVelY += day; }
private:
};

#endif // CHARACTER_H