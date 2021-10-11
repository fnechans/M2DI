#include "object.h"

#include <exception>

const char *const Object::dirName[4] = {"UP", "DOWN", "LEFT", "RIGHT"};

Object::Object(uint x, uint y)
{
    position.x = x;
    position.y = y;
    // DEFAULT hitbox based on tilesize, can be changed
    position.w = TILESIZEPHYSICS;
    position.h = TILESIZEPHYSICS;

    extVelX = 0;
    extVelY = 0;
    frictionX = TILESIZEPHYSICS / 8;
    frictionY = TILESIZEPHYSICS / 8;

    mapColor = {0, 0, 0, 0};
}

bool Object::on_screen(SDL_Rect *screen)
{
    return !(position.x * scaleRender < screen->x - (int) TILESIZERENDER 
            || position.x * scaleRender > screen->x + screen->w
            || position.y * scaleRender < screen->y -  (int) TILESIZERENDER
            || position.y * scaleRender > screen->y + screen->h
            );
}

void Object::plot(Window &window, SDL_Rect *screen)
{
    if (screen)
    {
        if(!on_screen(screen)) 
            return;
        posSX = position.x * scaleRender - screen->x;
        posSY = position.y * scaleRender - screen->y;
    }

    SDL_Rect renderRect = {posSX, posSY, (int) TILESIZERENDER, (int) TILESIZERENDER};
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

void Object::set_animation(std::string animationName)
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
    // whether to skip the actual plotting (but keeps the animation playing and sets up the posSX/Y)
    bool skipPlot = false;
    if (screen)
    {
        if (!on_screen(screen))
            skipPlot = true;
        posSX = position.x * scaleRender - screen->x;
        posSY = position.y * scaleRender - screen->y;
    }

    if (curAnimation != "")
    {
        auto &anim = animations[curAnimation];
        if (pause)
            anim.pause();
        SDL_Rect spriteRect = anim.get();
        if (pause)
            anim.play();
        if (skipPlot)
            return;
        SDL_Rect renderRect = {
            posSX + (int)(anim.shiftX * scaleRender),
            posSY + (int)(anim.shiftY * scaleRender),
            (int)(spriteRect.w * scaleRenderInput),
            (int)(spriteRect.h * scaleRenderInput)};
        // image->set_color(255,0,0);
        anim.image->render_image(window, &renderRect, &spriteRect, anim.angle, anim.flip);
    }
}