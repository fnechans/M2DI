#include "screen.h"

using Fnc = std::function<void()>;

void Screen::schedule_button_update(const std::string &name, std::function<void()> func)
{
    updates.push_back([name, func, this]()
        {
            if (menu->get_state(name))
            {
                func();
            }
        }
    );
}

void Screen::schedule_screen_position_update(Object *target)
{
    updates.push_back([this, target]()
                      {
        level->set_viewPort();
        level->set_map_screen_position(target);
    });
}

void Screen::schedule_screen_click_update(std::function<void()> func)
{
    updates.push_back([this, func]()
                      { if(level->screenClick) func(); });

}

void Screen::schedule_tile_click_update(std::function<void(Block&)> func)
{
    updates.push_back([this, func]()
                      {
    if(level->screenClick)
    {
        SDL_Rect mouseWorld = base::fromScreen(level->worldCoordinatesOnScreen,
                {level->bScreen.mouseX, level->bScreen.mouseY, 0, 0}, level->renderScale);
        for(auto &tile : level->get_map().get_tile_pointers())
        {
            if(tile->on_screen_quick(level->activeWorldCoordinates) &&
                tools::point_within_rect(mouseWorld.x, mouseWorld.y, tile->hitbox
                )
            )
            {
                func(*tile);
            }
        }
    }
    });
}

std::function<void()> Screen::l_quit()
{
    return [this]()
    { quit = true; };
}

std::function<void()> Screen::l_pause()
{
    return [this]()
    { level->pause = !level->pause; };
}

std::function<void()> Screen::l_property_update(const std::string &name, const PropertyType& value)
{
    try { std::cout << std::get<std::string>(value) << std::endl; } catch (...) {}
    return [this, name, value]()
    {
        properties.set(name, value);
    };
}

std::function<void()> Screen::l_property_toggle(const std::string &name)
{
    if(!std::holds_alternative<bool>(properties[name]))
    {
        std::cout << "property " << name << " is not a bool" << std::endl;
        return [](){};
    }
    return [this, name]()
    {
        properties.set(name, !properties.get<bool>(name));
    };
}

std::function<void()> Screen::l_nextScreen(const std::string &name)
{
    return [this, name]()
    { nextScreen = name; };
}

std::function<void()> Screen::l_screen_zoom(double factor)
{
    return [this, factor]()
        {
            level->renderScale *= factor;
        };
}

std::function<void(Block&)> Screen::l_update_tile_from_sprite(Sprites& sprites, std::string* name)
{
    return [&sprites, name](Block& tile)
    {
        tile.sprite = &sprites.at(*name);
    };
}

std::function<void()> Screen::l_fullscreen()
{
    return [this]()
    {
        window->toggleFullscreen();
    };
}

/// Plotting

void Screen::schedule_plot(std::function<void()> func)
{
    plots.push_back(func);
}

std::function<void()> Screen::l_plot_on_level(Level *viewport)
{
    return [viewport]()
    {
        viewport->set_viewPort();
    };
}

std::function<void()> Screen::l_plot_on_menu(Menu *menu)
{
    return [menu]()
    {
        menu->set_viewPort();
    };
}

std::function<void()> Screen::l_plot_image(IMG_wrapper *img, int x, int y, const bool &condition)
{
    return [this, &img, x, y, &condition]()
    {
        if(condition) img->render_image(*window, x, y);
    };
}
