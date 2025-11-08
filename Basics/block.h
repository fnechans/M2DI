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
    SDL_Rect shift;
    bool operator()() { 
        return std::all_of(checkers.begin(), checkers.end(), [](ValueChecker checker) { return checker(); }); 
    }
};

class Block : public HasProperties
{
public:
    Block(int x = 0, int y = 0, uint w = base::TILESIZEPHYSICS, uint h = base::TILESIZEPHYSICS);
    Block(const Block& other);
    Block(Block&&) = default;
    ~Block() {}

    SDL_Color mapColor;
    SDL_Rect* clip{nullptr};

    // global coor. vars
    SDL_Rect hitbox;
    SDL_Rect position(){ return {hitbox.x+hitbox.w/2, hitbox.y+hitbox.h/2, 1, 1}; }

    // switches
    bool doPlot{true};
    bool hasCollision{true};
    bool obscuresVision{true};

    // TODO: Following should probably be refactored out?

    IMG_wrapper* image{nullptr};
    void plot(Window &wrapper, SDL_Rect& screen, double renderScale);
    bool on_screen(SDL_Rect& screen, double renderScale);
    bool on_screen_quick(SDL_Rect& screen);

    void plot_animation(Window &window, SDL_Rect& screen, double renderScale, bool pause = false);
    void add_animation(Animation& animation, std::vector<std::pair<std::string, PropertyType>>& checkers, Fl_Rect shift);
    bool has_animation() { return !animations.empty(); }
    void set_animation();

private:
    std::vector <AnimationHelper> animations;
    AnimationHelper* currentAnimation{nullptr};

};


#endif // OBJECT_H
