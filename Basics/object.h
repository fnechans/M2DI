#ifndef OBJECT_H
#define OBJECT_H

#include "base.h"
#include "IMG_wrapper.h"
#include "animation.h"

#include <vector>
#include <map>

class Object : public base
{
public:
    Object(uint x = mWidth / 2, uint y = mHeight / 2);
    ~Object() {}

    enum direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    static const char *const dirName[4]; // = {"UP", "DOWN", "LEFT", "RIGHT"};

    SDL_Rect clip;

    // global coor. vars
    SDL_Rect hitbox;
    SDL_Rect position(){ return {hitbox.x+hitbox.w/2, hitbox.y+hitbox.h/2, 0, 0}; }

    // local coor. vard
    SDL_Rect positionScreen;
    // velocity based of external factors (knockback)
    float extVelX;
    float extVelY;
    // friction impacts extVel
    float frictionX;
    float frictionY;
    SDL_Color mapColor;

    std::shared_ptr<IMG_wrapper> image;
    bool set_image(Window &window, std::string imagePath);
    void plot(Window &wrapper, SDL_Rect *screen = nullptr);
    bool on_screen(SDL_Rect *screen);

    std::map<std::string, int> property;
    void set_health(uint value);
    void modify_health(int value);

    std::map<std::string, Animation> animations;
    void copy_animation(Object const &object);
    void plot_animation(Window &window, SDL_Rect *screen = nullptr, bool pause = false);
    void set_animation(const std::string& animationName);
    Animation&  get_current_animation() { return animations[curAnimation]; }
    const std::string&  get_current_animation_name() { return curAnimation; }

    bool dead = false;

    direction dir;
private:
    std::string curAnimation = "";
};


#endif // OBJECT_H
