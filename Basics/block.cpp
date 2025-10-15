#include "Basics/block.h"
#include "Tools/base.h"
#include "SDL_wrapper/IMG_wrapper.h"

#include <exception>
#include <variant>

Block::Block(uint x, uint y)
{
    hitbox.x = x;
    hitbox.y = y;
    // DEFAULT hitbox based on tilesize, can be changed
    hitbox.w = base::TILESIZEPHYSICS;
    hitbox.h = base::TILESIZEPHYSICS;

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

bool Block::on_screen(SDL_Rect *screen)
{
    if (!(hitbox.x * base::scaleRender() < screen->x - (int)base::TILESIZERENDER() || hitbox.x * base::scaleRender() > screen->x + screen->w || hitbox.y * base::scaleRender() < screen->y - (int)base::TILESIZERENDER() || hitbox.y * base::scaleRender() > screen->y + screen->h))
    {
        positionScreen = base::toScreen(screen, position());
        return true;
    }
    return false;
}

void Block::plot(Window &window, SDL_Rect *screen)
{
    if (screen)
    {
        if (!on_screen(screen))
            return;
    }

    int width = hitbox.w * base::scaleRender();
    int height = hitbox.h * base::scaleRender();
    SDL_Rect renderRect = {positionScreen.x - (int)width / 2, positionScreen.y - (int)height / 2, (int)width, (int)height};
    // image->set_color(255,0,0);
    image->render_image(window, &renderRect, &clip);
}

void Block::add_animation(Animation& animation, std::vector<std::pair<std::string, PropertyType>>& checkers)
{
    std::vector<ValueChecker> tmp_checkers;
    for (auto &checker : checkers)
    {
        tmp_checkers.push_back(properties.get_checker(checker.first, checker.second));
    }
    animations.push_back(AnimationHelper{std::move(animation), std::move(tmp_checkers)});
}

void Block::set_animation()
{
    for (auto &anim : animations)
    {
        if (anim())
        {
            if (currentAnimation == &anim.animation) return;
            if (currentAnimation) currentAnimation->stop();
            currentAnimation = &anim.animation;
            currentAnimation->play();
            break;
        }
    }
}

void Block::plot_animation(Window &window, SDL_Rect *screen, bool pause)
{
    // whether to skip the actual plotting (but keeps the animation playing and sets up the position_screen)
    bool skipPlot = false;
    if (screen)
    {
        if (!on_screen(screen))
            skipPlot = true;
    }


    if (currentAnimation)
    {
        auto &anim = *currentAnimation;
        if (pause)
            anim.pause();
        anim.run_and_plot(window, positionScreen, skipPlot);
        if (pause)
            anim.play();
    }
}