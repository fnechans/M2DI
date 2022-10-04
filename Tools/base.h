#ifndef BASE_H
#define BASE_H

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>

typedef unsigned int uint;

class base
{
public:
    base();

    static uint TICKS_PER_SECOND;
    static uint FRAMES_PER_SECOND;
    static float DELTA_T; // ms
    static bool VSYNC;

    static const uint TILESIZEPHYSICS = 1024; // defines physical size
                                            // so it is more related to
                                            // speed and such
    static const uint TILESIZEINPUT = 16;    // defines basic size of
                                            // tile in input
    static uint TILESIZERENDER;              // defines size of tile on screen
                                            // so can change if zooming in/out
    static double scaleRender;
    static double scaleRenderInput;

    static void set_tilerender(int);

    //Map dimension constants
    static uint mWidth;
    static uint mHeight;

    static SDL_Rect toScreen(SDL_Rect* screen, const SDL_Rect& position)
    {
        SDL_Rect positionScreen;
        positionScreen.x = position.x * scaleRender - screen->x;
        positionScreen.y = position.y * scaleRender - screen->y;
        return positionScreen;
    }

    static SDL_Rect fromScreen(SDL_Rect* screen, const SDL_Rect& positionScreen)
    {
        SDL_Rect position;
        position.x = (positionScreen.x + screen->x) / scaleRender;
        position.y = (positionScreen.y + screen->y) / scaleRender;
        return position;
    }
};

namespace tools
{
    bool key_down(SDL_Event &e, SDL_Keycode keycode);
    bool key_up(SDL_Event &e, SDL_Keycode keycode);
    bool point_within_rect(int x, int y, SDL_Rect rect);
    SDL_Rect get_endpoint(const SDL_Rect& origin, const SDL_Rect &direction, int range);
    bool line_intersect(const SDL_Rect &X1, const SDL_Rect &X2,
                        const SDL_Rect &Y1, const SDL_Rect &Y2);

    inline bool contains(const std::string& str, const std::string& str2 )
    {
        return (str.find(str2)!=std::string::npos);
    }

    // cleaup up functions, currently for "dead" chars
    // could be generalized in future if needed
    template <typename T>
    void remove_dead_vector(std::vector<T *> &objects)
    {
        objects.erase(std::remove_if(objects.begin(), objects.end(),
                                     [](T *&o)
                                     { return o->dead; }),
                      objects.end());
    }

    template <typename T>
    void remove_dead_vector(std::vector<T> &objects)
    {
        objects.erase(std::remove_if(objects.begin(), objects.end(),
                                     [](T &o)
                                     { return o.dead; }),
                      objects.end());
    }

    // For now <string,T>, generalize? Not needed right now...
    template <typename T>
    void remove_dead_map(std::map<std::string, T> &objects)
    {
        erase_if(objects, [](auto &item)
                 { return item.second.dead; });
    }

    template <typename ContainerT, typename PredicateT>
    void erase_if(ContainerT &items, const PredicateT &predicate)
    {
        for (auto it = items.begin(); it != items.end();)
        {
            if (predicate(*it))
                it = items.erase(it);
            else
                ++it;
        }
    }

    template <typename T>
    void reduce_to_zero(T &value, const T &reducer)
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


#endif