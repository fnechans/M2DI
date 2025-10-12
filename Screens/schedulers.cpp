#include "screen.h"

using Fnc = std::function<void()>;

void Screen::schedule_button_update(const std::string &name, std::function<void()> func)
{
    updates.push_back([name, func, this]()
                      { if (menu->get_state(name)) func(); });
}

void Screen::schedule_screen_position_update(Character *target)
{
    updates.push_back([this, target]()
                      {
        level->set_viewPort();
        level->get_map().screen_position(level->screenRect, level->viewPort, *target); });
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

std::function<void()> Screen::l_property_update(const std::string &name, const std::variant<bool, int, float, std::string>& value)
{
    return [this, name, value]()
    { properties.set(name, value); };
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
            // TODO: limits
            base::set_tilerender(base::TILESIZERENDER() * factor);
        };
}

/// Plotting

void Screen::schedule_plot(std::function<void()> func)
{
    updates.push_back(func);
}

std::function<void()> Screen::l_plot_viewport(Viewport *viewport)
{
    return [&viewport]()
    { 
        viewport->set_viewPort();
    };
}

std::function<void()> Screen::l_plot_image(IMG_wrapper *img, int x, int y)
{
    return [this, &img, x, y]()
    {
        img->render_image(*window, x, y);
    };
}

