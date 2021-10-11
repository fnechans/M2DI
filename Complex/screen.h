#ifndef ENGINE_H
#define ENGINE_H

#include <exception>

#include "menu.h"
#include "level.h"

class screen;

typedef std::shared_ptr<screen> screen_ptr;

class screen
{
public:
    screen() { window = std::make_shared<Window>(); }
    screen(std::shared_ptr<Window> _window) : window(_window) {}

    ~screen() {}
    std::shared_ptr<Window> window;
    SDL_Event event;

    virtual void user_init() {}
    virtual void user_evaluate() {}
    virtual void user_update() {}
    virtual void user_plot() {}
    virtual screen_ptr user_nextScreen() { return nullptr; }

    template <typename T>
    screen_ptr make_screen(bool copyWindow = false)
    {
        if (copyWindow)
            return std::make_shared<T>(window);
        return std::make_shared<T>();
    }

    Menu &add_menu(Menu::Position pos = Menu::RIGHT, SDL_Rect border = {0, base::TILESIZEINPUT * 12, base::TILESIZEINPUT * 12, 0})
    {
        menus.emplace_back(Menu(window.get(), pos, border));
        return menus.back();
    }
    Level &add_level()
    {
        levels.push_back(Level(window.get()));
        return levels.back();
    }

    screen_ptr loop()
    {
        if (!isInit)
            init();
        while (!quit)
        {
            // TODO: naming! preprocess vs reset()!
            for (auto &m : menus)
                m.reset();
            for (auto &l : levels)
                l.preprocess();

            // process inputs
            while (SDL_PollEvent(&event) != 0)
            {
                if (event.type == SDL_QUIT)
                {
                    quit = true;
                }
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    window->sWidth = event.window.data1;
                    window->sHeight = event.window.data2;
                }
                for (auto &l : levels)
                    l.evaluate(event);
                for (auto &m : menus)
                    m.evaluate(event);
                user_evaluate();
            }

            window->clear();
            user_update();

            for (auto &l : levels)
                l.plot();
            for (auto &m : menus)
                m.plot();

            user_plot();
            window->render();
        }

        return user_nextScreen();
    }

    bool quit = false;

private:
    bool isInit = false;

    std::string switchName = "";

    void init()
    {
        base::set_tilerender(64);
        // TODO: without else?
        if (!window->isInit && !window->init())
            throw std::runtime_error("Problem in init");

        user_init();
        isInit = true;
    }
    std::vector<Menu> menus;
    std::vector<Level> levels;
};
#endif