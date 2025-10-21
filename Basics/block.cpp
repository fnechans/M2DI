#include "Basics/block.h"
#include "Tools/base.h"
#include "SDL_wrapper/IMG_wrapper.h"

#include <exception>
#include <variant>

Block::Block(uint x, uint y, uint w, uint h)
{
    hitbox.x = x;
    hitbox.y = y;
    hitbox.w = w;
    hitbox.h = h;

    mapColor = {0, 0, 0, 0};
}

Block::Block(const Block &other)
{
    // TODO: this can be default copy constructor
    // once we remove animation
    hitbox = other.hitbox;

    mapColor = other.mapColor;
    clip = other.clip;
    image = other.image;

    doPlot = other.doPlot;
    hasCollision = other.hasCollision;
    obscuresVision = other.obscuresVision;
}

bool Block::on_screen(SDL_Rect& screen, double renderScale)
{
    if (!((hitbox.x+hitbox.w) * renderScale < screen.x || hitbox.x * renderScale > screen.x + screen.w ||
          (hitbox.y+hitbox.h) * renderScale < screen.y || hitbox.y * renderScale > screen.y + screen.h))
    {
        return true;
    }
    return false;
}

void Block::plot(Window &window, SDL_Rect& screen, double renderScale)
{
    if (!on_screen(screen, renderScale))
        return;

    SDL_Rect renderRect = base::toScreen(screen, hitbox, renderScale);
    image->render_image(window, &renderRect, &clip);
}

void Block::add_animation(Animation &animation, std::vector<std::pair<std::string, PropertyType>> &checkers, Fl_Rect shift)
{
    std::vector<ValueChecker> tmp_checkers;
    for (auto &checker : checkers)
    {
        tmp_checkers.push_back(properties.get_checker(checker.first, checker.second));
    }
    SDL_Rect real_shift = {shift.x * base::TILESIZEPHYSICS, shift.y * base::TILESIZEPHYSICS, shift.w * base::TILESIZEPHYSICS, shift.h * base::TILESIZEPHYSICS};
    animations.push_back(AnimationHelper{std::move(animation), std::move(tmp_checkers), real_shift});
}

void Block::set_animation()
{
    for (auto &anim : animations)
    {
        if (anim())
        {
            if (currentAnimation == &anim)
                return;
            if (currentAnimation)
                currentAnimation->animation.stop();
            currentAnimation = &anim;
            currentAnimation->animation.play();
            break;
        }
    }
}

void Block::plot_animation(Window &window, SDL_Rect& screen, double renderScale, bool pause)
{
    // whether to skip the actual plotting (but keeps the animation playing and sets up the position_screen)
    bool skipPlot = !on_screen(screen, renderScale);

    if (currentAnimation)
    {
        SDL_Rect positionScreen = base::toScreen(screen, position(), renderScale);
        auto &anim = currentAnimation->animation;
        SDL_Rect renderRect = currentAnimation->shift;
        renderRect.x *= renderScale;
        renderRect.y *= renderScale;
        renderRect.w *= renderScale;
        renderRect.h *= renderScale;
        renderRect.x += positionScreen.x;
        renderRect.y += positionScreen.y;
        if (pause)
            anim.pause();
        anim.run_and_plot(window, renderRect, skipPlot);
        if (pause)
            anim.play();
    }
}