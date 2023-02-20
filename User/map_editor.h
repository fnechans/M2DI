#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include "screen.h"
#include "preset.h"

class map_editor : public screen
{
public:
    using screen::screen;

    bool help = false;
    Level *level;
    Menu *menu;
    IMG_wrapper textHelp;
    Character *player;
    std::string curTile = "aa";
    uint sizeX = 40;
    uint sizeY = 36;
    std::vector<std::string> tileButtonNames;

    void user_init()
    {
        level = &add_level();
        int width = 40*base::TILESIZEINPUT;

        // TODO: do this properly, also needs minimal window size
        // (corresponding to the size of the menu = width)
        level->border = {0, 0, width, 0};
        level->position = Viewport::LEFT_FILL;
        level->set_map({0, 0, width, 0});
        level->get_map().init(sizeX, sizeY);
        level->add_image("map", "data/gfx/Overworld.png");
        level->set_map_image("map");
        level->get_map().load_map("", 160, 16);
        //level->get_map().load_map("data/start.map", 16, 16);
        //level->get_map().load_blocks("data/startBlock2.map", 16, 16);

        // character to only move around
        player = level->characters.add("player", 0., 0.);

        menu = &add_menu(Menu::RIGHT, {0, 0, width, 0});
        int buttonW = base::TILESIZEINPUT * 4;
        int buttonH = base::TILESIZEINPUT * 2;
        menu->add_image("button", "data/button.bmp");
        menu->add_image("map", "data/gfx/Overworld.png");

        menu->add_button("help", {0, 0, buttonW, buttonH});
        menu->set_button_image("help", "button", "HELP");

        menu->add_button("print", {buttonW, 0, buttonW, buttonH});
        menu->set_button_image("print", "button", "Print");

        menu->add_button("pause", {buttonW * 2, 0, buttonW, buttonH}, 2, 1, SDLK_p);
        menu->set_button_image("pause", "button", "pause");

        menu->add_button("+", {0, buttonH, buttonW, buttonH});
        menu->set_button_image("+", "button", "+");

        menu->add_button("-", {buttonW, buttonH, buttonW, buttonH});
        menu->set_button_image("-", "button", "-");

        menu->add_button("quit", {buttonW * 2, buttonH, buttonW, buttonH});
        menu->set_button_image("quit", "button", "EXIT");

        int corX = 0;
        int corY = 3;
        for(uint y = 0; y < sizeY; y++)
        {
            for(uint x = 0; x <= (uint) width/buttonH; x++)
            {
                if(y*sizeX+x>1000) break;
                std::string type = std::string(1, (char)'a'+x) + std::string(1, (char)'a'+y);
                menu->add_button(type, {corX*buttonH, corY*buttonH, buttonH, buttonH}, level->get_map().mappingClips.at(type));
                menu->set_button_image(type, "map");
                tileButtonNames.push_back(type);
                corX++;
                if(corX>width/buttonH){corX=0;corY++;}
            }
        }

        if (!textHelp.load_text(*window, "There is no help. You are on your own.", {0, 0, 0, 255}, 0, base::TILESIZERENDER() * 2))
            return;
    }

    void user_evaluate()
    {
        if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w: player->move_up(level->get_collisionObjects()); break;
            case SDLK_s: player->move_down(); break;
            case SDLK_a: player->move_left(); break;
            case SDLK_d: player->move_right(); break;
            }
        }
        else if (event.type == SDL_KEYUP && event.key.repeat == 0)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w: player->move_down(); break;
            case SDLK_s: player->move_up(level->get_collisionObjects()); break;
            case SDLK_a: player->move_right(); break;
            case SDLK_d: player->move_left(); break;
            }
        }
        player->hitbox.w = (level->viewPort.w / base::scaleRender() <  level->get_map().width())
            ? level->viewPort.w / base::scaleRender() : level->get_map().width()-1;
        player->hitbox.h = (level->viewPort.h / base::scaleRender() <  level->get_map().height())
            ? level->viewPort.h / base::scaleRender() : level->get_map().height()-1;
    }

    void user_update()
    {
        if (menu->get_state("pause"))
            level->pause = !level->pause;
        if (menu->get_state("help"))
            help = !help;
        if (menu->get_state("print"))
        {
            uint nTile = 0;
            for(auto& t : level->get_map().tiles)
            {
                std::cout << std::string(1, (char)'a'+t.clip.x/base::TILESIZEINPUT) + std::string(1, (char)'a'+t.clip.y/base::TILESIZEINPUT) << " ";
                if(++nTile%level->get_map().nTileX==0) std::cout << "\n";
            }
            std::cout << std::endl;
        }
        if (menu->get_state("quit"))
            quit = true;
        //   if(menu->get_state("fps")) showFPS = !showFPS;
        if (menu->get_state("+") && base::TILESIZERENDER() < 6 * base::TILESIZEINPUT)
            base::set_tilerender(base::TILESIZERENDER() * 2);
        if (menu->get_state("-") && base::TILESIZERENDER() > base::TILESIZEINPUT)
            base::set_tilerender(base::TILESIZERENDER() / 2);
        for(auto name : tileButtonNames)
            if (menu->get_state(name)) curTile = name;

        // move stuff (if not paused)
        if (!level->pause)
        {
            level->move_chars(DELTA_T);
        }

        level->get_map().screen_position(level->screenRect, level->viewPort, *player);
    }

    void user_plot()
    {
        if (help)
            textHelp.render_image(*window, 0, base::TILESIZERENDER() * 2);

        if (level->screenClick)
        {
            for(auto &tile : level->get_map().tiles)
            {
                if(tile.on_screen(&level->screenRect)
                    &&  tools::point_within_rect(level->bScreen.mouseX, level->bScreen.mouseY,
                            {tile.positionScreen.x, tile.positionScreen.y, (int) base::TILESIZERENDER(), (int) base::TILESIZERENDER()}
                        )
                )
                    tile.clip = level->get_map().mappingClips.at(curTile);
            }
        }
        player->plot_animation(*window, nullptr);
    }

    virtual screen_ptr user_nextScreen();
};

#endif