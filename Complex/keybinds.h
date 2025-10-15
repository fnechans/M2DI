#ifndef KEYBINDS_H
#define KEYBINDS_H

#include "SDL.h"

struct KeyBind
{
    KeyBind(SDL_Keycode key,
        std::function<void()> func_down,
        std::function<void()> func_up
    ) : key(key), func_down(func_down), func_up(func_up) {}
    SDL_Keycode key;
    std::function<void()> func_down;
    std::function<void()> func_up;
};

class KeyBinds
{
public:
    void add_keybind(const std::string &name,
            SDL_Keycode key,
            std::function<void()> func_down,
            std::function<void()> func_up = nullptr)
    {
        if (binds.count(name) > 0)
            throw std::runtime_error("Keybind of name " + name + " already "
                                                                 "exists in the manager.");
        binds.emplace(name, KeyBind(key, func_down, func_up));
    }
    void change_keybind(const std::string &name, SDL_Keycode key) { 
        binds.at(name).key = key;
    }

    void evaluate(SDL_Event &events)
    {
        if (events.type == SDL_KEYDOWN && events.key.repeat == 0)
        {
            for (auto &bind : binds)
            {
                if (bind.second.key == events.key.keysym.sym)
                {
//                    std::cout << "Keybind: " << bind.first << " pressed" << std::endl;
                    if (bind.second.func_down) bind.second.func_down();
                }
            }
        }
        else if (events.type == SDL_KEYUP && events.key.repeat == 0)
        {
            for (auto &bind : binds)
            {
                if (bind.second.key == events.key.keysym.sym)
                {
//                    std::cout << "Keybind: " << bind.first << " released" << std::endl;
                    if (bind.second.func_up) bind.second.func_up();
                }
            }
        }
    }

private:
    std::map<std::string, KeyBind> binds;
};

#endif // KEYBINDS_H
