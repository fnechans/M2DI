
#include "screen.h"
#include "top_down.h"
#include "main_menu.h"

#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

int main( int argc, char* args[] )
{
    (void)(argc);
    (void)(args);

    std::shared_ptr<screen> currentScreen = std::make_shared<main_menu>();
    while(currentScreen)
    {
        currentScreen = currentScreen->loop();
    }
}