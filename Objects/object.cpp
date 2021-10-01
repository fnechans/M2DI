#include "object.h"

#include <fstream>
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
    friction = TILESIZEPHYSICS / 16;

    spriteType = 0;
    mapColor = {0, 0, 0, 0};
    clips = std::make_shared<std::map<int, SDL_Rect>>();
}

void Object::plot(Window &window, SDL_Rect *screen)
{
    if (screen)
    {
        if (position.x * scaleRender < screen->x - TILESIZERENDER || position.x * scaleRender > screen->x + screen->w || position.y * scaleRender < screen->y - TILESIZERENDER || position.y * scaleRender > screen->y + screen->h)
            return;
        posSX = position.x * scaleRender - screen->x;
        posSY = position.y * scaleRender - screen->y;
    }

    SDL_Rect renderRect = {posSX, posSY, TILESIZERENDER, TILESIZERENDER};
    // image->set_color(255,0,0);
    image->render_image(window, &renderRect, &(*clips).at(spriteType));
}

bool Object::set_image(Window &window, std::string imagePath)
{
    image = std::make_shared<IMG_wrapper>();
    return image->load_media(window, imagePath.c_str());
}

// non-class functions

std::vector<Object> import_map(std::string mapFile, int mapSizeX, int mapSizeY)
{
    int x = 0;
    int y = 0;
    std::vector<Object> output;

    std::ifstream mapStream(mapFile);
    if (mapStream.fail())
        return {};
    else
    {
        int tileType = -1;
        for (int i = 0; i < mapSizeX * mapSizeY; ++i)
        {
            mapStream >> tileType;
            if (mapStream.fail())
            {
                printf("Error loading map: Unexpected end of file!\n");
                break;
            }

            if (tileType != -1)
            {
                output.push_back(Object(x, y));
                output.back().spriteType = tileType;
            }

            x += base::TILESIZEPHYSICS;
            if (x >= mapSizeX * base::TILESIZEPHYSICS)
            {
                x = 0;
                y += base::TILESIZEPHYSICS;
            }
        }
    }
    return output;
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
        if (position.x * scaleRender < screen->x - TILESIZERENDER || position.x * scaleRender > screen->x + screen->w || position.y * scaleRender < screen->y - TILESIZERENDER || position.y * scaleRender > screen->y + screen->h)
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