#ifndef BASE_H
#define BASE_H

// Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <variant>

typedef unsigned int uint;

namespace base
{

    const uint TILESIZEPHYSICS = 1024; // defines physical size
                                       // so it is more related to
                                       // speed and such
    const uint TILESIZEINPUT = 16;     // defines basic size of
                                       // tile in input
    uint TILESIZERENDER();             // defines size of tile on screen
                                       // so can change if zooming in/out
    double scaleRender();
    double scaleRenderInput();

    void set_tilerender(int);

    SDL_Rect toScreen(SDL_Rect *screen, const SDL_Rect &position);

    SDL_Rect fromScreen(SDL_Rect *screen, const SDL_Rect &positionScreen);
};


using PropertyType = std::variant<bool, int, float, std::string>;

class ValueChecker;

class Properties
{
public:
    uint count(const std::string &name) { return properties.count(name); }

    //    template <typename T>
    //    void set(const std::string &name, T value) { properties.emplace(name, value); }
    //
    void set(const std::string &name, PropertyType value) { properties.emplace(name, value); }


    template <typename T>
    T& set_and_get(const std::string &name, T value) { properties.emplace(name, value); return get<T>(name); }

    template <typename T>
    T& get(const std::string &name)
    {
        if (properties.count(name) == 0)
            throw std::runtime_error("Property of name " + name + " does "
                                                                  "not exists in the manager.");
        return std::get<T>(properties.at(name));
    }

    template <typename T>
    T *getp(const std::string &name)
    {
        if (properties.count(name) == 0)
            throw std::runtime_error("Property of name " + name + " does "
                                                                  "not exists in the manager.");
        return &std::get<T>(properties.at(name));
    }

    PropertyType& operator[](const std::string &name)
    {
        if (properties.count(name) == 0)
            throw std::runtime_error("Property of name " + name + " does "
                                                                  "not exists in the manager.");
        return properties.at(name);
    }

    ValueChecker get_checker(const std::string &name, const PropertyType& target); 


private:
    std::map<std::string, PropertyType> properties;
};

class HasProperties
{
public:
    Properties properties;
};

class ValueChecker
{
public:
    ValueChecker(PropertyType target, PropertyType& value) : target(target), value(value) {}
    const PropertyType target;
    PropertyType& value;
    bool operator()() { return value == target; }
};


#endif