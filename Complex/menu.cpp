#include "menu.h"


Menu::Menu(SDL_wrapper* _wrapper, Position pos, SDL_Rect bor) : position(pos), border(bor)
{
    wrapper = _wrapper;
}

void Menu::set_viewPort()
{
    switch (position)
    {
    case LEFT:
        viewPort = {0, border.y, sWidth-border.w, sHeight-border.h}; 
        break;
    case TOP:
        viewPort = {border.x, 0, sWidth-border.w, sHeight-border.h}; 
        break;
    case BOTTOM:
        viewPort = {border.x, sHeight-border.h, sWidth-border.w, 0}; 
        break;
    case CENTER:
        viewPort = {border.x, border.y, sWidth-border.w, sHeight-border.h}; 
        break;
    case RIGHT: // right is default
    default:
        viewPort = {sWidth-border.x, border.y, border.x, sHeight-border.y}; 
        break;
    } 
}

void Menu::evaluate(SDL_Event& event)
{
    set_viewPort();
    for(auto& button : buttons)
    {
        if(button.second.evaluate(event, viewPort) == button::CLICK
           || tools::key_down(event, buttonKeys[button.first])
        ) buttonState[button.first] = true;
    }
}

void Menu::reset()
{
    for(auto& button : buttons)
    {
        buttonState[button.first] = false;
    }
}

void Menu::plot()
{
    set_viewPort();
    SDL_RenderSetViewport( wrapper->gRenderer, &viewPort );
    for(auto& butIt : buttons)
    {
        auto& but = butIt.second;
        but.plot(*wrapper);
    }
}