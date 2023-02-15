#include "viewport.h"

Viewport::Viewport(Window *win, Position pos, SDL_Rect bor) : position(pos), border(bor)
{
    window = win;
}

void Viewport::set_viewPort()
{
    switch (position)
    {
    case LEFT:
        viewPort = {0, border.y, border.w,
                    window->sHeight - border.h};
        break;
    case LEFT_FILL:
        viewPort = {0, border.y, window->sWidth - border.w,
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
        viewPort = {window->sWidth / 2 - border.w / 2, border.y, border.w,
                    window->sHeight - border.h - border.y};
        break;
    case WHOLE:
        viewPort = {0, 0, window->sWidth, window->sHeight};
        break;
    case RIGHT: // right is default
    default:
        viewPort = {window->sWidth - border.w, border.y, border.w,
                    window->sHeight - border.h - border.y};
        break;
    }
    SDL_RenderSetViewport(window->sdlRenderer, &viewPort);
}
