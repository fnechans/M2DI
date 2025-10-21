#ifndef MENU_H
#define MENU_H

#include "Basics/viewport.h"
#include "Basics/button.h"
#include <map>
class Menu : public Viewport
{
public:
    Menu(Window *_wrapper, Position pos, SDL_Rect border);
    bool evaluate(SDL_Event &event); // returns true if event relevant to speed up/avoid multiple evals
    void reset();
    void plot();

    // Buttons
    void add_button(std::string name, SDL_Rect position, int ws = 2, int hs = 1, SDL_Keycode key = SDLK_UNKNOWN)
    {
        buttons.emplace(name, button(position, ws, hs));
        buttonKeys.emplace(name, key);
        buttonState.emplace(name, false);
    }
    void add_button(std::string name, SDL_Rect position, SDL_Rect clips[button::COUNT], SDL_Keycode key = SDLK_UNKNOWN)
    {
        buttons.emplace(name, button(position, clips));
        buttonKeys.emplace(name, key);
        buttonState.emplace(name, false);
    }
    void add_button(std::string name, SDL_Rect position, SDL_Rect clip, SDL_Keycode key = SDLK_UNKNOWN)
    {
        buttons.emplace(name, button(position, clip));
        buttonKeys.emplace(name, key);
        buttonState.emplace(name, false);
    }
    button& add_button2(std::string name, int x, int y, int w, int h, int w_sprite, int h_sprite, SDL_Keycode key = SDLK_UNKNOWN)
    {
        add_button(name, {x, y, w, h}, w_sprite, h_sprite, key);
        return buttons.at(name);
    }
    bool get_state(std::string name) { 
        if(buttonState.count(name)==0)
            throw std::runtime_error("Button of name "+name+" does "
                "not texists in the menu.");
        return buttonState.at(name);
    }



private:
    std::map<std::string, button> buttons;
    std::map<std::string, SDL_Keycode> buttonKeys;
    std::map<std::string, bool> buttonState;
};
#endif // MENU_H