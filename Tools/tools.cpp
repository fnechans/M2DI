#include "tools.h"

bool MLine::intersects(const MLine& l2, SDL_Rect* intersection )
{
    return tools::line_intersect(*this, l2, intersection);
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

    std::vector<MLine> get_lines(const SDL_Rect& rect)
    {
        return {
            {rect.x, rect.y, rect.x+rect.w, rect.y},
            {rect.x, rect.y, rect.x, rect.y+rect.h},
            {rect.x+rect.w, rect.y+rect.h, rect.x+rect.w, rect.y},
            {rect.x+rect.w, rect.y+rect.h, rect.x, rect.y+rect.h}
        };
    }
    std::vector<SDL_Rect> get_points(const SDL_Rect& rect)
    {
        return {
            {rect.x, rect.y, 0, 0},
            {rect.x+rect.w, rect.y, 0, 0},
            {rect.x, rect.y+rect.h, 0, 0},
            {rect.x+rect.w, rect.y+rect.h, 0, 0}
        };
    }
    bool line_intersect(const MLine &l1, const MLine &l2,
                        SDL_Rect* intersection)
    {
        long denom = (l1.p1.x - l1.p2.x) * (l2.p1.y - l2.p2.y) - (l1.p1.y - l1.p2.y) * (l2.p1.x - l2.p2.x);
        // if denom 0 then the lines are paralel:
        if (denom == 0)
            return false;

        long num1, num2;
        if (denom > 0)
        {
            num1 = (l1.p1.x - l2.p1.x) * (l2.p1.y - l2.p2.y) - (l1.p1.y - l2.p1.y) * (l2.p1.x - l2.p2.x);
            if (num1 < 0 || (num1 > denom && !l1.halfInfinite))
                return false;

            num2 = (l1.p1.x - l2.p1.x) * (l1.p1.y - l1.p2.y) - (l1.p1.y - l2.p1.y) * (l1.p1.x - l1.p2.x);
            if (num2 < 0 || (num2 > denom && !l2.halfInfinite))
                return false;

        }
        else // denom < 0
        {
            num1 = (l1.p1.x - l2.p1.x) * (l2.p1.y - l2.p2.y) - (l1.p1.y - l2.p1.y) * (l2.p1.x - l2.p2.x);
            if (num1 > 0 || (num1 < denom && !l1.halfInfinite))
                return false;

            num2 = (l1.p1.x - l2.p1.x) * (l1.p1.y - l1.p2.y) - (l1.p1.y - l2.p1.y) * (l1.p1.x - l1.p2.x);
            if (num2 > 0 || (num2 < denom && !l2.halfInfinite))
                return false;

        }

        // if lines intersect and intersection provided, determine the point
        if(intersection)
        {
            double t = ((double) num1)/denom;
            intersection->x = l1.p1.x + t*(l1.p2.x-l1.p1.x);
            intersection->y = l1.p1.y + t*(l1.p2.y-l1.p1.y);
        }
        return true;
    }
}