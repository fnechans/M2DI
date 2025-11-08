#include "level.h"

Level::Level(Window *win, Position pos, SDL_Rect bor) : Viewport(win, pos, bor), bScreen({0, 0, 0, 0})
{
}

void Level::bake()
{
    collisionObjects.clear();
    obscuringObjects.clear();
    damagableObjects.clear();
    for (auto obj : characters)
    {
        if(obj->hasCollision) collisionObjects.push_back(obj);
        if(obj->obscuresVision) obscuringObjects.push_back(obj);
        damagableObjects.push_back(obj); // TODO: simply return characters?
    }

    for (auto &t : currentBlocks->tiles)
    {
        if(t.hasCollision) collisionObjects.push_back(&t);
        if(t.obscuresVision) obscuringObjects.push_back(&t);
    }
    
    for (auto t : currentMap->map_border_colision())
    {
        collisionObjects.push_back(t);
    }
}

void Level::reset()
{
    characters.clean();
    projectiles.clean();
    bake();
}

bool Level::evaluate(SDL_Event &event)
{
    set_viewPort();
    bScreen.screenPos = viewPort;

    if(pause) return false;

    auto state = bScreen.evaluate(event, {0, 0, 0, 0});
    mousePositionScreen = {bScreen.mouseX, bScreen.mouseY, 0, 0};
    if (state == bScreen.CLICK)
    {
        screenClick = true;
        return true;
    }
    else if (state >= bScreen.UNCLICK)
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

    if(ai_active) follow_ai();

    for(auto obj : characters)
    {
        obj->move(collisionObjects, DELTA_T);
        obj->set_animation();
    }
    for(auto obj : projectiles)
    {
        obj->move(collisionObjects, DELTA_T);
        obj->set_animation();
    }
}

void Level::plot()
{
    set_viewPort();
    currentMap->render_map(*window, worldCoordinatesOnScreen, renderScale);
    currentBlocks->render_map(*window, worldCoordinatesOnScreen, renderScale);

    // TODO: are all collision objects needed here?

    // First plot paths and add characters
    // to plotable as objects printed on top of the map
    // In future other objects will be added here as well
    std::vector<Object *> plotable;
    for (auto obj : characters)
    {
        if (true) //obj->doPlotPath)
            obj->plot_path(*window, worldCoordinatesOnScreen, renderScale);
    }
    plotable.insert(plotable.end(), characters.get_obj().begin(), characters.get_obj().end());
    plotable.insert(plotable.end(), projectiles.get_obj().begin(), projectiles.get_obj().end());

    // plot objects from to bottom (overlap!)
    // TODO: is this the best way? feels slow - maybe check for nearby?
    std::sort(plotable.begin(), plotable.end(),
              [](Object* a, Object* b){
        return a->position().y < b->position().y;}
    );
    for(auto& obj : plotable)
    {
        if(!obj->doPlot) continue;

        if(obj->has_animation()) obj->plot_animation(*window, worldCoordinatesOnScreen, renderScale, pause);
        else obj->plot(*window, worldCoordinatesOnScreen, renderScale);
    }
    if(minimap){
        minimap->set_viewPort();
        currentMap->render_minimap(*window, worldCoordinatesOnMinimap, renderScaleMinimap);
        currentBlocks->render_minimap(*window, worldCoordinatesOnMinimap, renderScaleMinimap, collisionObjects);
    }
}