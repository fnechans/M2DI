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

    // Set window (also respondible for rendering)
    bool VSYNC = false;
    auto window = std::make_shared<Window>(VSYNC);

    // Set first screen and static settings
    std::shared_ptr<screen> currentScreen = std::make_shared<main_menu>(window);
    while (currentScreen)
    {
        currentScreen = currentScreen->loop();
    }
    return 0;
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
