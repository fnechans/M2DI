#include "menu.h"

Menu::Menu(Window *_wrapper, Position pos, SDL_Rect bor) : position(pos), border(bor)
{
    window = _wrapper;
}

void Menu::set_viewPort()
{
    switch (position)
    {
    case LEFT:
        viewPort = {0, border.y, border.w,
                    window->sHeight - border.h};
        break;
    case TOP:
        viewPort = {border.x, 0, window->sWidth - border.w,
                    window->sHeight - border.h - border.y};
        break;
    case BOTTOM:
        viewPort = {border.x, window->sHeight - border.h,
                    window->sWidth - border.w, 0};
        break;
    case CENTER:
        viewPort = {window->sWidth/2-border.w/2, border.y, border.w,
                    window->sHeight - border.h - border.y};
        break;
    case RIGHT: // right is default
    default:
        viewPort = {window->sWidth - border.w, border.y, border.w,
                    window->sHeight - border.h - border.y};
        break;
    }
}

void Menu::evaluate(SDL_Event &event)
{
    set_viewPort();
    for (auto &button : buttons)
    {
        if (button.second.evaluate(event, viewPort) == button::CLICK || tools::key_down(event, buttonKeys[button.first]))
            buttonState[button.first] = true;
    }
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
    SDL_RenderSetViewport(window->sdlRenderer, &viewPort);
    for (auto &butIt : buttons)
    {
        auto &but = butIt.second;
        but.plot(*window);
    }
}