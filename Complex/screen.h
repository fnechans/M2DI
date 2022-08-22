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

        // Ticks to ms conversion, round give closest integer
        Uint32 time_step_ms = std::round(1000. / base::TICKS_PER_SECOND);
        Uint32 next_game_step = SDL_GetTicks(); // initial value
        uint nRender = 0;
        uint nEval = 0;
        Uint32 fps_step = next_game_step;

        while (!quit)
        {
            Uint32 now = SDL_GetTicks();

            // Check so we don't render for no reason (unless vsync is enabled)
            if(next_game_step <= now || vsync){
                int computer_is_too_slow_limit = 10; // max number of advances per render, adjust this according to your minimum playable fps

                // Loop until all steps are executed or computer_is_too_slow_limit is reached
                while((next_game_step <= now) && (computer_is_too_slow_limit--)){
                    // TODO: naming! preprocess vs reset()!
                    nEval++;
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

                    user_update();

                    next_game_step += time_step_ms; // count 1 game tick done
                }

                nRender++;
                if(now-fps_step > 10000)
                {
                    std::cout << "fps expected: " << fps << " / real: " << ((float) nRender*1000)/(now-fps_step) << std::endl;
                    std::cout << "ticks expected: " << base::TICKS_PER_SECOND <<  " / real: " << ((float) nEval*1000)/(now-fps_step) << std::endl;
                    nRender = 0;
                    nEval = 0;
                    fps_step = now;
                }
                window->clear();

                for (auto &l : levels)
                    l.plot();
                for (auto &m : menus)
                    m.plot();

                user_plot();
                window->render();
            } else {
                // we're too fast, wait a bit.
                if(true){ // false to burn cpu
                    SDL_Delay(next_game_step - now);
                }
            }
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

    uint fps = 60; // does nothing right now
    bool vsync = true;
};
#endif