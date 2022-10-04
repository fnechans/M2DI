#include "base.h"

uint base::mWidth(0);
uint base::mHeight(0);
uint base::TILESIZERENDER(0);
uint base::TICKS_PER_SECOND(128);
uint base::FRAMES_PER_SECOND(60);
float base::DELTA_T(1. / base::TICKS_PER_SECOND);
bool base::VSYNC(false);
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

    bool point_within_rect(int x, int y, SDL_Rect rect)
    {
        return (x>=rect.x && x<=rect.x+rect.w && y>=rect.y && y<=rect.y+rect.h);
    }

    SDL_Rect get_endpoint(const SDL_Rect& origin, const SDL_Rect &direction, int range)
    {
        int dX = direction.x - origin.x;
        int dY = direction.y - origin.y;
        int dR = (int) sqrt(dX*dX + dY*dY);

        if(dR==0) return {origin.x, origin.y, 0, 0};

        return {origin.x + range * dX / dR, origin.y + range * dY / dR, 0, 0};
    }

    bool line_intersect(const SDL_Rect &X1, const SDL_Rect &X2,
                        const SDL_Rect &Y1, const SDL_Rect &Y2)
    {
        long denom = (X1.x - X2.x) * (Y1.y - Y2.y) - (X1.y - X2.y) * (Y1.x - Y2.x);
        // if denom 0 then the lines are paralel:
        if (denom == 0)
            return false;

        if (denom > 0)
        {
            long num1 = (X1.x - Y1.x) * (Y1.y - Y2.y) - (X1.y - Y1.y) * (Y1.x - Y2.x);
            if (num1 < 0 || num1 > denom)
                return false;

            long num2 = (X1.x - Y1.x) * (X1.y - X2.y) - (X1.y - Y1.y) * (X1.x - X2.x);
            if (num2 < 0 || num2 > denom)
                return false;

        }
        else // denom < 0
        {
            long num1 = (X1.x - Y1.x) * (Y1.y - Y2.y) - (X1.y - Y1.y) * (Y1.x - Y2.x);
            if (num1 > 0 || num1 < denom)
                return false;

            long num2 = (X1.x - Y1.x) * (X1.y - X2.y) - (X1.y - Y1.y) * (X1.x - X2.x);
            if (num2 > 0 || num2 < denom)
                return false;

        }
        return true;
    }
}