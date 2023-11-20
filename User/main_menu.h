#ifndef MAINMENU_H
#define MAINMENU_H

#include "screen.h"

class main_menu : public screen
{
public:
    using screen::screen;
    main_menu() {}

    Menu *menu;
    void user_init()
    {
        menu = &add_menu(Menu::CENTER, {0, base::TILESIZEINPUT*2, base::TILESIZEINPUT * 12, 0});
        int buttonW = base::TILESIZEINPUT * 12;
        int buttonH = base::TILESIZEINPUT * 4;
        menu->add_image("button", "data/button.bmp");

        menu->add_button("topdown", {0, buttonH, buttonW, buttonH});
        menu->set_button_image("topdown", "button", "Top-down fighter");

        // TODO: Fix editor, now broken!!
        //menu->add_button("editor", {0, buttonH * 3, buttonW, buttonH});
        //menu->set_button_image("editor", "button", "Map editor");

        menu->add_button("quit", {0, buttonH * 5, buttonW, buttonH});
        menu->set_button_image("quit", "button", "EXIT");
    }
    bool help = false;
    bool start_topDown = false;
    bool start_editor = false;

    void user_update()
    {
        if (menu->get_state("topdown"))
        {
            start_topDown = true;
            quit = true;
        }
        //if (menu->get_state("editor"))
        //{
        //    start_editor = true;
        //    quit = true;
        //}
        if (menu->get_state("quit"))
            quit = true;
    }

    virtual screen_ptr user_nextScreen();
};

#endif