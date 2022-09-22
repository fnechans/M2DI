#include "screen.h"
#include "top_down.h"
#include "main_menu.h"
#include "map_editor.h"

#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

int main(int argc, char *args[])
{
    (void)(argc);
    (void)(args);

    base::TICKS_PER_SECOND = 200;
    base::FRAMES_PER_SECOND = 60;
    base::DELTA_T = 1./ base::TICKS_PER_SECOND;
    base::VSYNC = false;
    std::shared_ptr<screen> currentScreen = std::make_shared<main_menu>();
    while (currentScreen)
    {
        currentScreen = currentScreen->loop();
    }
}

screen_ptr main_menu::user_nextScreen()
{
    if (start_topDown)
        return make_screen<top_down_fight>(true);
    if (start_editor)
        return make_screen<map_editor>(true);

    return nullptr;
}

screen_ptr top_down_fight::user_nextScreen()
{
    return make_screen<main_menu>(true);
}

screen_ptr map_editor::user_nextScreen()
{
    return make_screen<main_menu>(true);
}
