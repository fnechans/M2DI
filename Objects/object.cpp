#include "object.h"

#include <exception>

const char *const Object::dirName[4] = {"UP", "DOWN", "LEFT", "RIGHT"};

Object::Object(uint x, uint y)
{
    hitbox.x = x;
    hitbox.y = y;
    // DEFAULT hitbox based on tilesize, can be changed
    hitbox.w = TILESIZEPHYSICS;
    hitbox.h = TILESIZEPHYSICS;

    extVelX = 0;
    extVelY = 0;
    frictionX = 4;
    frictionY = 4;

    mapColor = {0, 0, 0, 0};
}

bool Object::on_screen(SDL_Rect *screen)
{
    if( !(hitbox.x * scaleRender < screen->x - (int) TILESIZERENDER
            || hitbox.x * scaleRender > screen->x + screen->w
            || hitbox.y * scaleRender < screen->y -  (int) TILESIZERENDER
            || hitbox.y * scaleRender > screen->y + screen->h
            ))
    {
        positionScreen = toScreen(screen, hitbox);
        return true;
    }
    return false;
}

void Object::plot(Window &window, SDL_Rect *screen)
{
    if (screen)
    {
        if(!on_screen(screen))
            return;
    }

    SDL_Rect renderRect = {positionScreen.x, positionScreen.y, (int) TILESIZERENDER, (int) TILESIZERENDER};
    // image->set_color(255,0,0);
    image->render_image(window, &renderRect, &clip);
}

bool Object::set_image(Window &window, std::string imagePath)
{
    image = std::make_shared<IMG_wrapper>();
    return image->load_media(window, imagePath.c_str());
}



void Object::set_health(uint value)
{
    property["health"] = value;
    property["max_health"] = value;
}

void Object::modify_health(int value)
{
    if(property.count("health") == 0) return; // skip if object does not have health
    property["health"] += value;
    if (property["health"] > property["max_health"])
        property["health"] = property["max_health"];
    if (property["health"] < 0)
        property["health"] = 0;
    if (property["health"] == 0)
        dead = true;
}

void Object::copy_animation(Object const &object)
{
    for (auto &iter : object.animations)
    {
        if (animations.find(iter.first) != animations.end())
            std::cout << "Warning: Animation " << iter.first << " already exists!\n";
        animations[iter.first] = Animation(iter.second);
    }
}

void Object::set_animation(const std::string& animationName)
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

void Object::plot_animation(Window &window, SDL_Rect *screen, bool pause)
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