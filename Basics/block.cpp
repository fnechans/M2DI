#include "block.h"

#include <exception>


Block::Block(uint x, uint y)
{
    hitbox.x = x;
    hitbox.y = y;
    // DEFAULT hitbox based on tilesize, can be changed
    hitbox.w = base::TILESIZEPHYSICS;
    hitbox.h = base::TILESIZEPHYSICS;

    mapColor = {0, 0, 0, 0};
}

Block::Block(const Block& other)
{
    // TODO: this can be default copy constructor
    // once we remove animation
    hitbox = other.hitbox;

    mapColor = other.mapColor;
    clip = other.clip;
    image = other.image;
    copy_animation(other);
    curAnimation = other.curAnimation;

    doPlot = other.doPlot;
    hasCollision = other.hasCollision;
    obscuresVision = other.obscuresVision;
}

bool Block::on_screen(SDL_Rect *screen)
{
    if( !(hitbox.x * base::scaleRender() < screen->x - (int) base::TILESIZERENDER()
            || hitbox.x * base::scaleRender() > screen->x + screen->w
            || hitbox.y * base::scaleRender() < screen->y -  (int) base::TILESIZERENDER()
            || hitbox.y * base::scaleRender() > screen->y + screen->h
            ))
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
        if(!on_screen(screen))
            return;
    }

    int width = hitbox.w*base::scaleRender();
    int height = hitbox.h*base::scaleRender();
    SDL_Rect renderRect = {positionScreen.x - (int) width/2, positionScreen.y - (int) height/2, (int) width, (int) height};
    // image->set_color(255,0,0);
    image->render_image(window, &renderRect, &clip);
}

bool Block::set_image(Window &window, std::string imagePath)
{
    image = std::make_shared<IMG_wrapper>();
    return image->load_media(window, imagePath.c_str());
}


void Block::copy_animation(Block const &object)
{
    for (auto &iter : object.animations)
    {
        if (animations.find(iter.first) != animations.end())
            std::cout << "Warning: Animation " << iter.first << " already exists!\n";
        animations[iter.first] = Animation(iter.second);
    }
}

void Block::set_animation(const std::string& animationName)
{
    if (animationName == curAnimation)
        return;

    if (curAnimation != "")
        animations[curAnimation].stop();
    if (animations.find(animationName) == animations.end())
        throw std::runtime_error("Animation with " + animationName + " does not exist!");

    animations[animationName].play();
    curAnimation = animationName;
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

    if (curAnimation != "")
    {
        auto &anim = animations[curAnimation];
        if (pause)
            anim.pause();
        anim.run_and_plot(window, positionScreen, skipPlot);
        if (pause)
            anim.play();
    }
}