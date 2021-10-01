#include "base.h"

int base::mWidth(0);
int base::mHeight(0);
int base::TILESIZERENDER(0);
double base::scaleRender;
double base::scaleRenderInput;

base::base()
{
}

void base::set_tilerender(int TR)
{
    TILESIZERENDER = TR;
    scaleRender = ((double)TILESIZERENDER) / TILESIZEPHYSICS;
    scaleRenderInput = ((double)TILESIZERENDER) / TILESIZEINPUT;
}

namespace tools
{
    bool key_down(SDL_Event &e, SDL_Keycode keycode)
    {
        return e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == keycode;
    }

    bool key_up(SDL_Event &e, SDL_Keycode keycode)
    {
        return e.type == SDL_KEYUP && e.key.repeat == 0 && e.key.keysym.sym == keycode;
    }
    void reduce_to_zero(int &value, const int &reducer)
    {
        if (value > 0)
        {
            value -= reducer;
            if (value < 0)
                value = 0;
        }
        if (value < 0)
        {
            value += reducer;
            if (value > 0)
                value = 0;
        }
    }
}