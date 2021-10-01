#ifndef MENU_H
#define MENU_H

#include "base.h"
#include "button.h"
#include <map>
#include "IMG_wrapper.h"
#include "timer.h"

class Menu : base
{
public:
    enum Position {CENTER, RIGHT, LEFT, TOP, BOTTOM};
    
    Menu(Window* _wrapper, Position pos = RIGHT, SDL_Rect border = {TILESIZEINPUT*12, TILESIZEINPUT*12, 0, 0}); 
    void set_viewPort();
    void evaluate(SDL_Event& event);
    void reset();
    void plot();

    // Images
    void add_image(std::string name, std::string imagePath)
    {
        images.emplace(name, std::make_shared<IMG_wrapper>());
        images[name]->load_media(*window,imagePath.c_str());

    }

    // Buttons
    void add_button(std::string name, SDL_Rect position, SDL_Keycode key = SDLK_UNKNOWN, float ws = 2, float hs = 1)
    {
        buttons.emplace(name, button(position, ws, hs));
        buttonKeys.emplace(name, key);
        buttonState.emplace(name, false);
    }
    void set_button_image(std::string name, std::string imgName, std::string text="", uint textSize =  TILESIZEINPUT*2, SDL_Color textColor = {255,255,150,255})
    {
        buttons.at(name).image = images[imgName];
        if( text!="" )
        {
            buttons.at(name).text = std::make_shared< IMG_wrapper>();
            buttons.at(name).text->load_text(*window, text, textColor, textSize);
        }
    }
    bool get_state(std::string name){return buttonState.at(name);}
private:
    Window * window;
    SDL_Rect viewPort;
    Position position;
    SDL_Rect border;
    std::map<std::string, std::shared_ptr<IMG_wrapper>> images;
    std::map<std::string, button> buttons;
    std::map<std::string, SDL_Keycode> buttonKeys;
    std::map<std::string, bool> buttonState;
};
#endif // MENU_H