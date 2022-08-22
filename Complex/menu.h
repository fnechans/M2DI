#ifndef MENU_H
#define MENU_H

#include "viewport.h"
#include "button.h"
#include <map>
#include "IMG_wrapper.h"
#include "timer.h"

class Menu : public Viewport
{
public:

    Menu(Window *_wrapper, Position pos = RIGHT, SDL_Rect border = {0, TILESIZEINPUT * 12, TILESIZEINPUT * 12, 0});
    void evaluate(SDL_Event &event);
    void reset();
    void plot();

    // Images
    void add_image(std::string name, std::string imagePath)
    {
        images.emplace(name, std::make_shared<IMG_wrapper>());
        images[name]->load_media(*window, imagePath.c_str());
    }

    // Buttons
    void add_button(std::string name, SDL_Rect position, float ws = 2, float hs = 1, SDL_Keycode key = SDLK_UNKNOWN)
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
    void set_button_image(std::string name, std::string imgName, std::string text = "", SDL_Color textColor = {255, 255, 150, 255})
    {
        auto& curButton = buttons.at(name);
        curButton.image = images.at(imgName);
        if (text != "")
        {
            curButton.text = std::make_shared<IMG_wrapper>();
            // 0.9 for the last argument (width of text) to leave place for button border
            curButton.text->load_text(*window, text, textColor, curButton.clips[0].h*scaleRenderInput, curButton.clips[0].w*scaleRenderInput*10);
        }
    }
    bool get_state(std::string name) { return buttonState.at(name); }

private:
    std::map<std::string, std::shared_ptr<IMG_wrapper>> images;
    std::map<std::string, button> buttons;
    std::map<std::string, SDL_Keycode> buttonKeys;
    std::map<std::string, bool> buttonState;
};
#endif // MENU_H