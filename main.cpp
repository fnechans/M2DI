
#include "screen.h"
#include "top_down.h"
#include "main_menu.h"

#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

int main(int argc, char *args[])
{
    (void)(argc);
    (void)(args);

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
    else
        return nullptr;
}

screen_ptr top_down_fight::user_nextScreen()
{
    return make_screen<main_menu>(true);
}