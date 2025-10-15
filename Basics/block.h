#ifndef BLOCK_H
#define BLOCK_H

#include "SDL_wrapper/animation.h"

#include <vector>
#include <map>
#include <variant>


struct AnimationHelper
{
    Animation animation;
    std::vector<ValueChecker> checkers;
    bool operator()() { 
        return std::all_of(checkers.begin(), checkers.end(), [](ValueChecker checker) { return checker(); }); 
    }
};

class Block : public HasProperties
{
public:
    Block(uint x = 0, uint y = 0);
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

    // switches
    bool doPlot{true};
    bool hasCollision{true};
    bool obscuresVision{true};

    // TODO: Following should probably be refactored out?

    IMG_wrapper* image;
    void plot(Window &wrapper, SDL_Rect *screen = nullptr);
    bool on_screen(SDL_Rect *screen);

    void plot_animation(Window &window, SDL_Rect *screen = nullptr, bool pause = false);
    void add_animation(Animation& animation, std::vector<std::pair<std::string, PropertyType>>& checkers);
    bool has_animation() { return !animations.empty(); }
    void set_animation();

private:
    std::vector <AnimationHelper> animations;
    Animation* currentAnimation{nullptr};

};


#endif // OBJECT_H
