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
            int real_factor = base::TILESIZERENDER() * factor;
            if (real_factor > 0)
                base::set_tilerender(real_factor);
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

// prepsat -> vse do Objectu
// init -> definitovat ktere properties ovlivni animaci (vector string)
// pridani animace pak je vektor pozadovanych vlastnosti
// vlastnoti by meli mit flag jestli se zmenili -> get by mel dat true,
// false se pak bude delat pri resetu
// otazka je jestli  mit moved/direction jako properties
// nebo emmber promenne. Ten kod by mohl proste uklada vector
// pointeru a pro moved/direction by meli vyjimku pri init
// otazka jestli to pak neomzeit na int (a bool)
// taky otazka jestli umoznit "na danem parametru nezalezi"
// ale to by pak vedlo k ambiguite? Jaky by byl priklad?