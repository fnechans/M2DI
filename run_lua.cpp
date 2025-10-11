#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <iostream>

#include "api.h"
#include <filesystem>
namespace fs = std::filesystem;

std::string run_screen(sol::state &lua, const std::string &dirname, std::shared_ptr<Window> window)
{
    std::cout << "Running: " << dirname << std::endl;

    std::cout << "=== load screen ===" << std::endl;
    lua["screen"] = std::make_unique<Screen>(window);
    lua.script_file("lua/" + dirname + "/init.lua");
    Screen &mainMenu = lua["screen"];

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
    lua.open_libraries();//sol::lib::base, sol::lib::package);

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
    sleep(1);
    lua["imageManager"] = std::make_unique<ImageManager>(*window);
    ImageManager &imageManager = lua["imageManager"];

    std::cout << "=== load images ===" << std::endl;
    std::vector<std::string> screens;
    std::string path = "lua/";
    std::string dirname = "top_down";
    for (const auto &entry : fs::directory_iterator(path))
    {
        std::string tmp_dirname = entry.path().filename();
        std::cout << "Loading images for: " << tmp_dirname << std::endl;
        lua.script_file("lua/" + tmp_dirname + "/image.lua");
        screens.push_back(tmp_dirname);
    }

    std::cout << "=== run lua ===" << std::endl;
    while (true)
    {
        dirname = run_screen(lua, dirname, window);
        if (dirname == "")
            break;
        if (std::find(screens.begin(), screens.end(), dirname) == screens.end())
            throw std::runtime_error("Invalid screen: " + dirname);

        imageManager.delete_texts();
    }
    return 0;
}
