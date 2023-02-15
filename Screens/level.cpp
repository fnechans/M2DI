#include "level.h"

Level::Level(Window *win, Position pos, SDL_Rect bor) : Viewport(win, pos, bor), bScreen({0, 0, 0, 0})
{
}

void Level::bake()
{
    collisionObjects.clear();
    damagableObjects.clear();
    for (auto obj : characters)
    {
        collisionObjects.push_back(obj);
        damagableObjects.push_back(obj);
    }

    for (auto &t : curMap->blocks)
    {
        collisionObjects.push_back(&t);
    }
    
    for (auto t : curMap->map_border_colision())
    {
        collisionObjects.push_back(t);
    }
}

void Level::reset()
{
    characters.clean();
    grenades.clean();
    bake();
}

bool Level::evaluate(SDL_Event &event)
{
    set_viewPort();
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

void Level::move_chars(double DELTA_T)
{
    if (pause)
        return;
    for(auto obj : characters)
    {
        obj->move(collisionObjects, DELTA_T);
    }
    for(auto obj : grenades)
    {
        obj->move(collisionObjects, DELTA_T);
    }
}

void Level::plot_map()
{
    set_viewPort();
    curMap->render_map(*window, screenRect);
}

void Level::plot()
{
    set_viewPort();
    // TODO: are all collision objects needed here?

    // First plot paths and add characters
    // to plotable as objects printed on top of the map
    // In future other objects will be added here as well
    std::vector<Object *> plotable;
    for (auto obj : characters)
    {
        if (obj->doPlotPath)
            obj->plot_path(*window, &screenRect);
    }
    plotable.insert(plotable.end(), characters.get_obj().begin(), characters.get_obj().end());
    plotable.insert(plotable.end(), grenades.get_obj().begin(), grenades.get_obj().end());

    // plot objects from to bottom (overlap!)
    // TODO: is this the best way? feels slow - maybe check for nearby?
    std::sort(plotable.begin(), plotable.end(),
              [](Object* a, Object* b){
        return a->position().y < b->position().y;}
    );
    for(auto& obj : plotable)
    {
        if(!obj->doPlot) continue;

        if(obj->get_current_animation_name()!="") obj->plot_animation(*window, &screenRect, pause);
        else obj->plot(*window, &screenRect);
    }

    curMap->render_minimap(*window, collisionObjects);
}