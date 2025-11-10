#include "tilemap.h"

void Map_wrapper::add_chunk(Chunk chunk)
{
    chunks.push_back(std::move(chunk));
    boundBox.x = std::min(boundBox.x, chunk.boundBox.x);
    boundBox.y = std::min(boundBox.y, chunk.boundBox.y);
    boundBox.w = std::max(boundBox.w, chunk.boundBox.w);
    boundBox.h = std::max(boundBox.h, chunk.boundBox.h);
    adjust_screen();
}

void Map_wrapper::render_map(Window &window, SDL_Rect &mapPosition, double renderScale)
{
    for (auto &chunk : chunks)
    {
        if (chunk.active)
            chunk.plot(window, mapPosition, renderScale);
    }
}

void Map_wrapper::render_minimap(Window &window, SDL_Rect &minimapPosition, double renderScale, const std::vector<Block *> &objects)
{
    for (auto &chunk : chunks)
    {
        if(chunk.active)
            chunk.plot_minimap(window, minimapPosition, renderScale);
    }

    for (auto &obj : objects)
    {
        if (obj->sprite == nullptr || !obj->on_screen(minimapPosition, renderScale))
            return;
        SDL_Rect renderRect = base::toScreen(minimapPosition, obj->hitbox, renderScale);
        window.drawColorRect(&renderRect, obj->sprite->color);
    }
}

std::vector<Block *>& Map_wrapper::map_border_colision()
{
    return border;
}

void Map_wrapper::screen_position(SDL_Rect &worldCoordinatesOnScreen, SDL_Rect &viewPort, Block &obj, double renderScale)
{
    SDL_Rect objPositionScreen;
    objPositionScreen.x = viewPort.w / 2;
    objPositionScreen.y = viewPort.h / 2;

    worldCoordinatesOnScreen.x = obj.position().x * renderScale - objPositionScreen.x;
    worldCoordinatesOnScreen.y = obj.position().y * renderScale - objPositionScreen.y;

    if (worldCoordinatesOnScreen.x < boundBox.x * renderScale)
    {
        worldCoordinatesOnScreen.x = boundBox.x * renderScale;
    }
    else if (worldCoordinatesOnScreen.x + (viewPort.w) > boundBox.w * renderScale && boundBox.w * renderScale > (viewPort.w))
    {
        worldCoordinatesOnScreen.x = boundBox.w * renderScale - (viewPort.w);
    }

    if (worldCoordinatesOnScreen.y < boundBox.y * renderScale)
    {
        worldCoordinatesOnScreen.y = boundBox.y * renderScale;
    }
    else if (worldCoordinatesOnScreen.y + (viewPort.h) > boundBox.h * renderScale && boundBox.h * renderScale > (viewPort.h))
    {
        worldCoordinatesOnScreen.y = boundBox.h * renderScale - (viewPort.h);
    }

    worldCoordinatesOnScreen.w = viewPort.w;
    worldCoordinatesOnScreen.h = viewPort.h;
}

void Map_wrapper::adjust_screen()
{
    borderLeft.hitbox = {-base::TILESIZEPHYSICS + boundBox.x, -base::TILESIZEPHYSICS + boundBox.y,
                         base::TILESIZEPHYSICS, 2*base::TILESIZEPHYSICS + boundBox.h - boundBox.y};
    borderRight.hitbox = {boundBox.w, -base::TILESIZEPHYSICS,
                          base::TILESIZEPHYSICS, 2*base::TILESIZEPHYSICS + boundBox.h - boundBox.y};
    borderTop.hitbox = {boundBox.x, - base::TILESIZEPHYSICS + boundBox.h, boundBox.w - boundBox.x, base::TILESIZEPHYSICS};
    borderBottom.hitbox = {boundBox.x, -base::TILESIZEPHYSICS + boundBox.y, boundBox.w - boundBox.x, base::TILESIZEPHYSICS};
    border.clear(); // Necessary? getting paranoid ...
    border = {&borderLeft, &borderRight, &borderTop, &borderBottom};
}

std::vector<Block *> &Map_wrapper::get_tile_pointers(bool activeOnly)
{
    tilePointers.clear();
    for (auto &chunk : chunks)
    {
        if(activeOnly && !chunk.active)
            continue;
        for (auto &tile : chunk.tiles)
            tilePointers.push_back(&tile);
    }
    return tilePointers;
}
