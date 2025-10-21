#include "base.h"


SDL_Rect base::toScreen(SDL_Rect& screen, const SDL_Rect &position, double renderScale)
{
    SDL_Rect positionScreen;
    positionScreen.x = position.x * renderScale - screen.x;
    positionScreen.y = position.y * renderScale - screen.y;
    positionScreen.w = position.w * renderScale;
    positionScreen.h = position.h * renderScale;
    return positionScreen;
}

SDL_Rect base::fromScreen(SDL_Rect& screen, const SDL_Rect &positionScreen, double renderScale)
{
    SDL_Rect position;
    position.x = (positionScreen.x + screen.x) / renderScale;
    position.y = (positionScreen.y + screen.y) / renderScale;
    position.w = positionScreen.w / renderScale;
    position.h = positionScreen.h / renderScale;
    return position;
}

ValueChecker Properties::get_checker(const std::string &name, const PropertyType& target) 
{ 
    if (properties.count(name) == 0)
        throw std::runtime_error("Property of name " + name + " does "
                                                              "not exists in the manager.");
    return ValueChecker(target, properties.at(name)); 
}