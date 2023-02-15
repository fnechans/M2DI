#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "base.h"
#include "window.h"

class Viewport
{
public:
    enum Position
    {
        CENTER,
        RIGHT,
        LEFT,
        TOP,
        BOTTOM,
        WHOLE,
        RIGHT_FILL,
        LEFT_FILL,
        TOP_FILL,
        BOTTOM_FILL
    };

    Viewport(Window *win, Position pos, SDL_Rect bor);

    void set_viewPort();

    Window *window;
    SDL_Rect viewPort;
    Position position;
    SDL_Rect border;
};

#endif