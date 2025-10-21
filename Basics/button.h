#ifndef BUTTON_H
#define BUTTON_H

#include "Tools/base.h"
#include "SDL_wrapper/IMG_wrapper.h"

class button
{
public:
    enum clipType
    {
        DEFAULT,
        HOVER,
        CLICK,
        UNCLICK,
        COUNT
    };
    button(SDL_Rect pos, int ws = 2, int hs = 1);
    button(SDL_Rect pos, SDL_Rect _clips[COUNT]);
    button(SDL_Rect pos, SDL_Rect clip);
    button(const button &b);
    ~button() {}


    SDL_Rect clips[COUNT];

    SDL_Rect screenPos;
    uint state = DEFAULT;

    IMG_wrapper* image;
    IMG_wrapper* text;
    void plot(Window &wrapper);
    int evaluate(SDL_Event &event, SDL_Rect viewPort);

    int mouseX = 0;
    int mouseY = 0;
};

#endif //BUTTON_H