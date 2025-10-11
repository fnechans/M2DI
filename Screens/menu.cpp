#include "menu.h"
#include "Tools/tools.h"

Menu::Menu(Window *win, Position pos, SDL_Rect bor) : Viewport(win, pos, bor)
{
}

bool Menu::evaluate(SDL_Event &event)
{
    set_viewPort();
    for (auto &button : buttons)
    {
        if (button.second.evaluate(event, viewPort) == button::CLICK || tools::key_down(event, buttonKeys[button.first]))
        {
            buttonState[button.first] = true;
            return true;
        }
    }
    return false;
}

void Menu::reset()
{
    for (auto &button : buttons)
    {
        buttonState[button.first] = false;
    }
}

void Menu::plot()
{
    set_viewPort();
    for (auto &butIt : buttons)
    {
        auto &but = butIt.second;
        but.plot(*window);
    }
}