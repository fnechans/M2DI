#ifndef OBJECT_H
#define OBJECT_H

#include "block.h"

class Object : public Block
{
public:
    Object(uint x = 0, uint y = 0, uint w = base::TILESIZEPHYSICS, uint h = base::TILESIZEPHYSICS);

    Object(const Object& other);

    enum direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    static inline const char *const dirName[4]{"UP", "DOWN", "LEFT", "RIGHT"};

    /// Properties
    int& health;
    int& max_health;
    bool& dead;
    bool& moved;
    bool& bounced;
    int& dir;

    /// Health

    void set_health(int value);
    void modify_health(int value);

    /// MOVEMENT

    // Type of movement
    // TopDown - WSAD like, moves in any direction
    // Side - WS+jump
    enum MoveType {TopDown, Sidescroll};
    MoveType moveType = TopDown;
    void move_left();
    void move_right();
    void move_up(const std::vector<Block *> &collObjects);
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
    void plot_path(Window &wrapper, SDL_Rect& screen, double renderScale);

    std::vector<Block *> path = {};
    Object *target{nullptr};

    void move(const std::vector<Block *> &collObjects, double DELTA_T);
    bool does_collide(SDL_Rect &pos, const std::vector<Block *>& collObjects);
    bool next_to(SDL_Rect pos, direction dir, const std::vector<Block *>& collObjects); // here copy of pos(ition) on purpose!
    void set_target(Object *target){ this->target = target; }

    // intrinsic speed of the character
    double intrVelX{0.};
    double intrVelY{0.};
    double speedX{4.};
    double speedY{4.};
    // velocity based of external factors (knockback)
    double extVelX{0.};
    double extVelY{0.};
    // friction impacts extVel
    double frictionX{4.};
    double frictionY{4.};
    double bounceFactor = 0; // fraction of velocity object gets back when colliding with wall
    bool doPlotPath{false};
    std::string dir_name(){ return dirName[dir]; }

    void kick(const SDL_Rect& dir, double multiplier);
    void kick(double dax, double day){ extVelX += dax; extVelY += day; }
private:
};

std::function<void()> l_player_move_up(Object *player);
std::function<void()> l_player_jump_up(Object *player, const std::vector<Block *>& collision_objects);
std::function<void()> l_player_move_down(Object *player);
std::function<void()> l_player_move_left(Object *player);
std::function<void()> l_player_move_right(Object *player);
std::function<void()> l_player_stop_updown(Object *player);
std::function<void()> l_player_stop_leftright(Object *player);

#endif // CHARACTER_H