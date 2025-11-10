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
    Screen() :
        quit(properties.set_and_get<bool>("quit", false)),
        showFPS(properties.set_and_get<bool>("showFPS", false))
    { window = std::make_shared<Window>(); }
    Screen(std::shared_ptr<Window> _window) :
    quit(properties.set_and_get<bool>("quit", false)),
    showFPS(properties.set_and_get<bool>("showFPS", false)),
    window(_window)
    {}
    bool& quit;
    bool& showFPS;

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

    Menu *add_menu(Menu::Position pos, SDL_Rect border)
    {
        menu = std::make_unique<Menu>(window.get(), pos, border);
        return menu.get();
    }
    Menu *add_menu2(Menu::Position pos, int x, int y, int w, int h)
    {
        add_menu(pos, {x, y, w, h});
        return menu.get();
    }
    Level &add_level(Level::Position pos = Level::WHOLE,
                    SDL_Rect bor = {0, 0, 0, 0})
    {
        level = std::make_unique<Level>(window.get(),
                                        pos,
                                        bor);
        return *level;
    }

    void evaluate();
    screen_ptr loop();
    uint currentFPS{0};
    uint currentTickrate{0};

    // TODO: make ticks/frames per second configurable
    static inline uint TICKS_PER_SECOND{1000};
    static inline uint FRAMES_PER_SECOND{60};
    double DELTA_T{1. / TICKS_PER_SECOND};

    void init()
    {
        // TODO: without else?
        if (!window->isInit && !window->init())
            throw std::runtime_error("Problem in init");

        isInit = true;
    }
    std::unique_ptr<Menu> menu;
    std::unique_ptr<Level> level;
    std::string nextScreen = "";

    void schedule_button_update(const std::string &name, std::function<void()> func);
    void schedule_screen_position_update(Object *target);
    void schedule_screen_click_update(std::function<void()> func);
    void schedule_tile_click_update(std::function<void(Block&)> func);
    std::function<void()> l_screen_zoom(double factor);
    std::function<void()> l_quit();
    std::function<void()> l_pause();
    std::function<void()> l_property_update(const std::string &name, const PropertyType& value);
    std::function<void()> l_property_toggle(const std::string &name);
    std::function<void()> l_nextScreen(const std::string &name);
    std::function<void(Block&)> l_update_tile_from_sprite(Sprites& sprites, std::string* name);
    std::function<void()> l_fullscreen();

    void schedule_plot(std::function<void()> func);
    std::function<void()> l_plot_on_level(Level *viewport);
    std::function<void()> l_plot_on_menu(Menu *menu);
    std::function<void()> l_plot_image(IMG_wrapper *img, int x, int y, const bool &condition);
    std::function<void()> l_set_animation(Object *object, const std::string &name, std::function<bool()> condition);

    KeyBinds keybinds;

private:
    bool isInit = false;
    std::vector<std::function<void()>> updates;
    std::vector<std::function<void()>> plots;
};
#endif