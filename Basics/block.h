#ifndef BLOCK_H
#define BLOCK_H

#include "base.h"
#include "IMG_wrapper.h"
#include "animation.h"

#include <vector>
#include <map>

class Block
{
public:
    Block(uint x = 0, uint y = 0);
  //  Block(Block& other) = default;
    Block(const Block& other);
    Block(Block&&) = default;
    ~Block() {}

    SDL_Color mapColor;
    SDL_Rect clip;

    // global coor. vars
    SDL_Rect hitbox;
    SDL_Rect position(){ return {hitbox.x+hitbox.w/2, hitbox.y+hitbox.h/2, 1, 1}; }

    // local coor. vard
    SDL_Rect positionScreen;

    bool doPlot{true};
    std::shared_ptr<IMG_wrapper> image;
    bool set_image(Window &window, std::string imagePath);
    void plot(Window &wrapper, SDL_Rect *screen = nullptr);
    bool on_screen(SDL_Rect *screen);

    std::map<std::string, int> property;

    std::map<std::string, Animation> animations;
    void copy_animation(Block const &object);
    void plot_animation(Window &window, SDL_Rect *screen = nullptr, bool pause = false);
    void set_animation(const std::string& animationName);
    Animation&  get_current_animation() { return animations[curAnimation]; }
    const std::string&  get_current_animation_name() { return curAnimation; }

private:
    std::string curAnimation = "";

};


#endif // OBJECT_H
