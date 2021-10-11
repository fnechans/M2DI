#include "level.h"

Level::Level(Window *_wrapper) : window(_wrapper), bScreen({0, 0, 0, 0})
{
}

void Level::bake()
{
    collisionObjects.clear();
    for (auto &c : characters)
    {
        collisionObjects.push_back(&c.second);
    }
    for (auto &t : curMap.blocks)
    {
        collisionObjects.push_back(&t);
    }
}
void Level::preprocess()
{
    screenClick = false;

    tools::remove_dead_vector<Object>(curMap.blocks);
    tools::remove_dead_map<Character>(characters);
    bake();
}

void Level::evaluate(SDL_Event &event)
{
    bScreen.screenPos = curMap.gameplayScreen;

    if (bScreen.evaluate(event, {0, 0, 0, 0}) == bScreen.CLICK)
        screenClick = true;
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
    curMap.render_map(*window, screenRect);
    // TODO: are all collision objects needed here?

    for (auto &cIt : characters)
    {
        Character *chr = &cIt.second;
        chr->plot_path(*window,&screenRect);
        chr->plot_animation(*window, &screenRect, pause);
    }
    curMap.render_minimap(*window, collisionObjects);
}