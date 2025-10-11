#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <iostream>

#include "Screens/screen.h"
#include "Managers/image_manager.hpp"

void init_enums(sol::state &lua)
{
    lua.new_enum("Position",
                 "CENTER", Menu::Position::CENTER,
                 "RIGHT", Menu::Position::RIGHT,
                 "LEFT", Menu::Position::LEFT,
                 "TOP", Menu::Position::TOP,
                 "BOTTOM", Menu::Position::BOTTOM,
                 "WHOLE", Menu::Position::WHOLE,
                 "RIGHT_FILL", Menu::Position::RIGHT_FILL,
                 "LEFT_FILL", Menu::Position::LEFT_FILL,
                 "TOP_FILL", Menu::Position::TOP_FILL,
                 "BOTTOM_FILL", Menu::Position::BOTTOM_FILL);
    lua["TILESIZEINPUT"] = base::TILESIZEINPUT;
}
static void create_classes(sol::state &lua)
{
    {
        sol::usertype<button> cl = lua.new_usertype<button>("button");
        cl["image"] = &button::image;
        cl["text"] = &button::text;
        cl["screenPosition"] = &button::screenPos;
    }
    // SDL
    {
        auto cl = lua.new_usertype<SDL_Rect>("SDL_Rect", sol::constructors<SDL_Rect(), SDL_Rect(int, int, int, int)>());
        cl["x"] = &SDL_Rect::x;
        cl["y"] = &SDL_Rect::y;
        cl["w"] = &SDL_Rect::w;
        cl["h"] = &SDL_Rect::h;
    }
    {
        lua.new_usertype<SDL_Color>("SDL_Color", sol::constructors<SDL_Color(), SDL_Color(uint8_t, uint8_t, uint8_t, uint8_t)>());
    }
    {
        sol::usertype<Menu> cl = lua.new_usertype<Menu>("Menu");
        cl["add_button2"] = &Menu::add_button2;
        cl["get_state"] = &Menu::get_state;
    }
    {
        sol::usertype<Screen> cl = lua.new_usertype<Screen>("Screen");
        cl["add_menu2"] = &Screen::add_menu2;
        cl["quit"] = &Screen::quit;
        cl["nextScreen"] = &Screen::nextScreen;
        cl["schedule_button_update"] = &Screen::schedule_button_update;
        cl["l_quit"] = &Screen::l_quit;
        cl["l_nextScreen"] = &Screen::l_nextScreen;
    }
    {
        // make usertype metatable
        sol::usertype<ImageManager> cl = lua.new_usertype<ImageManager>("ImageManager");
        cl["add_image"] = &ImageManager::add_image;
        cl["get_image"] = &ImageManager::get_image;
        cl["delete_image"] = &ImageManager::delete_image;
        cl["add_text"] = &ImageManager::add_text;
        cl["get_text"] = &ImageManager::get_text;
        cl["delete_text"] = &ImageManager::delete_text;
    }
}

std::string run_screen(sol::state &lua, const std::string &dirname, std::shared_ptr<Window> window)
{
    std::cout << "Running: " << dirname << std::endl;

    std::cout << "=== load screen ===" << std::endl;
    lua["screen"] = std::make_unique<Screen>(window);
    lua.script_file("lua/" + dirname + "/init.lua");
    Screen &mainMenu = lua["screen"];

    lua.script_file("lua/" + dirname + "/update.lua");
    std::cout << "=== run screen ===" << std::endl;
    mainMenu.loop();
    std::cout << "=== done ===" << std::endl;
    return mainMenu.nextScreen;
}

int main(int argc, char *args[])
{
    std::cout << "=== opening a state ===" << std::endl;

    sol::state lua;
    // open some common libraries
    lua.open_libraries(sol::lib::base, sol::lib::package);

    std::cout << std::endl;

    std::cout << "=== init interface ===" << std::endl;
    init_enums(lua);
    create_classes(lua);

    (void)(argc);
    (void)(args);

    std::cout << "=== create window ===" << std::endl;
    // Set window (also respondible for rendering)
    bool VSYNC = false;
    auto window = std::make_shared<Window>(VSYNC);
    window->init();
    lua["imageManager"] = std::make_unique<ImageManager>(*window);
    ImageManager &imageManager = lua["imageManager"];

    std::cout << "=== load images ===" << std::endl;
    std::string dirname = "main_menu";
    lua.script_file("lua/" + dirname + "/image.lua");

    std::cout << "=== run lua ===" << std::endl;
    while (true)
    {
        dirname = run_screen(lua, dirname, window);
        if (dirname == "")
            break;
    }
    return 0;
}
