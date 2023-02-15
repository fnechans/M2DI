#ifndef TOOLS_H
#define TOOLS_H

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>

class MLine
{
public:
    MLine(){}
    MLine(const SDL_Rect &X1, const SDL_Rect &X2) : p1(X1), p2(X2) {}
    MLine(int a1, int b1, int a2, int b2)
    {
        p1.x = a1;
        p1.y = b1;
        p2.x = a2;
        p2.y = b2;
    }
    bool intersects(const MLine& l2, SDL_Rect* intersection = nullptr);
    bool halfInfinite = false; // if true then p2 only define direction
    SDL_Rect p1, p2;
};

namespace tools
{
    inline void print_rect(const SDL_Rect& rect){ std::cout << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << "\n"; }

    inline bool point_equal(const SDL_Rect& rect, const SDL_Rect& rect2){return rect.x==rect2.x && rect.y==rect2.y;}

    bool key_down(SDL_Event &e, SDL_Keycode keycode);
    bool key_up(SDL_Event &e, SDL_Keycode keycode);
    bool point_within_rect(int x, int y, SDL_Rect rect);
    SDL_Rect get_endpoint(const SDL_Rect& origin, const SDL_Rect &direction, int range);
    std::vector<MLine> get_lines(const SDL_Rect& rect);
    std::vector<SDL_Rect> get_points(const SDL_Rect& rect);
    inline float distance2(const SDL_Rect& rect, const SDL_Rect& rect2)
    { return (rect.x-rect2.x)*(rect.x-rect2.x) + (rect.y-rect2.y)*(rect.y-rect2.y);}
    bool line_intersect(const MLine& l1, const MLine& l2, SDL_Rect* intersection = nullptr);

    template <typename T>
    inline bool contains(std::vector<T> vec, T val)
    {
        return std::find(vec.begin(), vec.end(), val) != vec.end();
    }

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
    void remove_dead_map(std::map<std::string, T*> &objects)
    {
        erase_if(objects, [](auto &item)
                 { return item.second->dead; });
    }

    // For now <string,T>, generalize? Not needed right now...
    template <typename T>
    void remove_dead_map(std::map<std::string, T> &objects)
    {
        erase_if(objects, [](auto &item)
                 { return item.second.dead; });
    }

    // For now <string,T>, generalize? Not needed right now...
    template <typename T>
    void remove_dead_map_ptr(std::map<std::string, T> &objects)
    {
        erase_if(objects, [](auto &item)
                 { return item.second->dead; });
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