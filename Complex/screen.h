#ifndef ENGINE_H
#define ENGINE_H

#include <exception>

#include "menu.h"
#include "level.h"

class screen
{
public:
    screen(){}
    ~screen(){}
    // TODO: move wrapper to funct
    static SDL_wrapper wrapper;

    virtual void user_init(){}
    virtual void user_evaluate(){}
    virtual void user_update(){}
    virtual void user_plot(){}
    virtual std::shared_ptr<screen> user_nextScreen(){return nullptr;}

    Menu& add_menu(Menu::Position pos = Menu::RIGHT, SDL_Rect border = {base::TILESIZEINPUT*12, base::TILESIZEINPUT*12, 0, 0})
    { 
        menus.emplace_back(Menu(&wrapper, pos, border)); 
        return menus.back();
    }
    Level& add_level() {
         levels.push_back(std::move(Level(&wrapper)));
         return levels.back(); 
    }

    std::shared_ptr<screen> loop()
    {
        if (!isInit)
            init();
        while (!quit )
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
                for (auto &l : levels)
                    l.evaluate(event);
                for (auto &m : menus)
                    m.evaluate(event);
                user_evaluate();
            }

            for (auto &l : levels)
                l.move_chars();
            user_update();

            wrapper.clear();
            for (auto &l : levels)
                l.plot(); 
            for (auto &m : menus)
                m.plot();

            user_plot();
            wrapper.render();
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
        if( !wrapper.isInit && !wrapper.init() ) throw std::runtime_error("Problem in init");

        user_init();
        isInit = true;
    }
    std::vector<Menu> menus;
    std::vector<Level> levels;
    SDL_Event event;
};
#endif