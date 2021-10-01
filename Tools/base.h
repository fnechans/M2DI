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

class base
{
public:
    base();

    static const int TILESIZEPHYSICS = 128; // defines physical size
                                     // so it is more related to
                                     // speed and such
    static const int TILESIZEINPUT = 16; // defines basic size of
                                         // tile in input
    static int TILESIZERENDER; // defines size of tile on screen
                               // so can change if zooming in/out
    static double scaleRender;
    static double scaleRenderInput;

    static void set_tilerender(int);

    //Map dimension constants
    static int mWidth;
    static int mHeight;
};


namespace tools{
    bool key_down(SDL_Event & e, SDL_Keycode keycode);
    bool key_up(SDL_Event & e, SDL_Keycode keycode);
    // cleaup up functions, currently for "dead" chars
    // could be generalized in future if needed
    template<typename T>
    void remove_dead_vector( std::vector<T*> & objects )
    {
        objects.erase(std::remove_if(objects.begin(), objects.end(),
                      [](T*&o)
                      { return o->dead; }),
                      objects.end());
    }

    template<typename T>
    void remove_dead_vector( std::vector<T> & objects )
    {
        objects.erase(std::remove_if(objects.begin(), objects.end(),
                      [](T& o)
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
}

#endif
