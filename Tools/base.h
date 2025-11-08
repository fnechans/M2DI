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
#include <functional>
#include "tracy/Tracy.hpp" // Required

typedef unsigned int uint;

namespace base
{

    const uint TILESIZEPHYSICS = 1024; // defines physical size
                                       // so it is more related to
                                       // speed and such

    SDL_Rect toScreen(SDL_Rect& screen, const SDL_Rect &position, double renderScale);

    SDL_Rect fromScreen(SDL_Rect& screen, const SDL_Rect &positionScreen, double renderScale);
};


using PropertyType = std::variant<bool, int, double, std::string>;

class ValueChecker;

class Properties
{
public:
    uint count(const std::string &name) { return propertyMap.count(name); }

    //    template <typename T>
    //    void set(const std::string &name, T value) { propertyMap.emplace(name, value); }
    //
    void set(const std::string &name, PropertyType value) {
        if (propertyMap.count(name) == 0)
            propertyMap.emplace(name, value); 
        else 
            propertyMap.at(name) = value;
    }


    template <typename T>
    T& set_and_get(const std::string &name, T value) { propertyMap.emplace(name, value); return get<T>(name); }

    template <typename T>
    T& get(const std::string &name)
    {
        if (propertyMap.count(name) == 0)
            throw std::runtime_error("Property of name " + name + " does "
                                                                  "not exists in the manager.");
        return std::get<T>(propertyMap.at(name));
    }

    template <typename T>
    T *getp(const std::string &name)
    {
        if (propertyMap.count(name) == 0)
            throw std::runtime_error("Property of name " + name + " does "
                                                                  "not exists in the manager.");
        return &std::get<T>(propertyMap.at(name));
    }

    PropertyType& operator[](const std::string &name)
    {
        if (propertyMap.count(name) == 0)
            throw std::runtime_error("Property of name " + name + " does "
                                                                  "not exists in the manager.");
        return propertyMap.at(name);
    }

    ValueChecker get_checker(const std::string &name, const PropertyType& target); 


private:
    std::map<std::string, PropertyType> propertyMap;
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