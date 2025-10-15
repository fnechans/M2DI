#ifndef ENGINE_H
#define ENGINE_H

#include <exception>
#include <algorithm>

#include "menu.h"
#include "level.h"
#include "Complex/keybinds.h"

class Screen;

typedef std::shared_ptr<Screen> screen_ptr;

class Screen: public HasProperties
{
public:
    Screen() { window = std::make_shared<Window>(); }
    Screen(std::shared_ptr<Window> _window) : window(_window) {}

    ~Screen() {}
    std::shared_ptr<Window> window;
    SDL_Event event;

    virtual void user_finish() {}
    virtual screen_ptr user_nextScreen() { return nullptr; }

    template <typename T>
    screen_ptr make_screen(bool copyWindow = false)
    {
        if (copyWindow)
            return std::make_shared<T>(window);
        return std::make_shared<T>();
    }

    Menu *add_menu(Menu::Position pos = Menu::RIGHT, SDL_Rect border = {0, base::TILESIZEINPUT * 12, base::TILESIZEINPUT * 12, 0})
    {
        menu = std::make_unique<Menu>(window.get(), pos, border);
        return menu.get();
    }
    Menu *add_menu2(Menu::Position pos = Menu::RIGHT, int x = 0, int y = 0, int w = base::TILESIZEINPUT * 12, int h = 0)
    {
        add_menu(pos, {x, y, w, h});
        return menu.get();
    }
    Level &add_level()
    {
        level = std::make_unique<Level>(window.get());
        return *level;
    }

    void evaluate();
    screen_ptr loop();

    bool quit = false;
    uint currentFPS;
    uint currentTickrate;

    static inline uint TICKS_PER_SECOND{200};
    static inline uint FRAMES_PER_SECOND{60};
    double DELTA_T{1. / TICKS_PER_SECOND};

    void init()
    {
        base::set_tilerender(64);
        // TODO: without else?
        if (!window->isInit && !window->init())
            throw std::runtime_error("Problem in init");

        isInit = true;
    }
    std::unique_ptr<Menu> menu;
    std::unique_ptr<Level> level;
    std::string nextScreen = "";

    void schedule_button_update(const std::string &name, std::function<void()> func);
    void schedule_screen_position_update(Character *target);
    std::function<void()> l_screen_zoom(double factor);
    std::function<void()> l_quit();
    std::function<void()> l_pause();
    std::function<void()> l_property_update(const std::string &name, const std::variant<bool, int, float, std::string>& value);
    std::function<void()> l_nextScreen(const std::string &name);

    void schedule_plot(std::function<void()> func);
    std::function<void()> l_plot_on_level(Level *viewport);
    std::function<void()> l_plot_on_menu(Menu *menu);
    std::function<void()> l_plot_image(IMG_wrapper *img, int x, int y, const bool &condition);
    std::function<void()> l_set_animation(Character *object, const std::string &name, std::function<bool()> condition);

    KeyBinds keybinds;

private:
    bool isInit = false;
    std::vector<std::function<void()>> updates;
    std::vector<std::function<void()>> plots;
};
#endif