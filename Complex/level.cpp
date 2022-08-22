#include "level.h"

Level::Level(Window *win, Position pos, SDL_Rect bor) : Viewport(win, pos, bor), bScreen({0, 0, 0, 0})
{
}

void Level::bake()
{
    collisionObjects.clear();
    for (auto &c : characters)
    {
        collisionObjects.push_back(&c.second);
    }
    for (auto &t : curMap->blocks)
    {
        collisionObjects.push_back(&t);
    }
}
void Level::preprocess()
{
    //screenClick = false;

    tools::remove_dead_vector<Object>(curMap->blocks);
    tools::remove_dead_map<Character>(characters);
    bake();
}

void Level::evaluate(SDL_Event &event)
{
    bScreen.screenPos = viewPort;

    auto state = bScreen.evaluate(event, {0, 0, 0, 0});
    if (state == bScreen.CLICK)
        screenClick = true;
    else if (state == bScreen.UNCLICK)
        screenClick = false;
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

    for (auto &cIt : characters)
    {
        Character *chr = &cIt.second;
        chr->plot_path(*window,&screenRect);
        chr->plot_animation(*window, &screenRect, pause);
    }
    curMap->render_minimap(*window, collisionObjects);
}