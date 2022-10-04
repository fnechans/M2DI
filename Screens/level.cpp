#include "level.h"

Level::Level(Window *win, Position pos, SDL_Rect bor) : Viewport(win, pos, bor), bScreen({0, 0, 0, 0})
{
}

void Level::bake()
{
    collisionObjects.clear();
    damagableObjects.clear();
    for (auto &c : characters)
    {
        collisionObjects.push_back(&c.second);
        damagableObjects.push_back(&c.second);
    }
    for (auto &t : curMap->blocks)
    {
        collisionObjects.push_back(&t);
    }
}

void Level::reset()
{
    tools::remove_dead_map<Character>(characters);
    bake();
}

bool Level::evaluate(SDL_Event &event)
{
    bScreen.screenPos = viewPort;

    auto state = bScreen.evaluate(event, {0, 0, 0, 0});
    mousePositionScreen = {bScreen.mouseX, bScreen.mouseY, 0, 0};
    if (state == bScreen.CLICK)
    {
        screenClick = true;
        return true;
    }
    else if (state == bScreen.UNCLICK)
    {
        screenClick = false;
        return true;
    }
    return false;
}

void Level::move_chars()
{
    if (pause)
        return;

    for (auto &cIt : characters)
    {
        Character *chr = &cIt.second;
        chr->move(collisionObjects);
    }
}

void Level::plot()
{
    set_viewPort();
    curMap->render_map(*window, screenRect);
    // TODO: are all collision objects needed here?

    // First plot paths and add characters
    // to plotable as objects printed on top of the map
    // In future other objects will be added here as well
    std::vector<Object *> plotable;
    for (auto &cIt : characters)
    {
        Character *chr = &cIt.second;
        chr->plot_path(*window,&screenRect);
        plotable.push_back(chr);
    }

    // plot objects from to bottom (overlap!)
    std::sort(plotable.begin(), plotable.end(),
              [](Object* a, Object* b){return a->position().y < b->position().y;}
    );
    for(auto& obj : plotable) obj->plot_animation(*window, &screenRect, pause);

    curMap->render_minimap(*window, collisionObjects);
}