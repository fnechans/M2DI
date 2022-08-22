#ifndef TOPDOWN_H
#define TOPDOWN_H

#include "screen.h"
#include "preset.h"

#include "main_menu.h"

class main_menu;

class top_down_fight : public screen
{
public:
    using screen::screen;

    bool help = false;
    bool showFPS = false;
    Level *level;
    Menu *menu;
    IMG_wrapper playerHealth;
    std::stringstream textHealth;
    IMG_wrapper textHelp;
    std::stringstream streamFPS;
    IMG_wrapper textFPS;
    Character *player;
    int AIclick = 0; // how often is AI updated
    std::unique_ptr<AStar<Object *>> acko = nullptr;
    enum tiles
    {
        DIRT,
        GRASS,
        STONES,
        WOOD,
        WATTER
    };
    bool tick(uint frequency) { return AIclick % frequency == 0; }
    void init_astar()
    {
        std::vector<Object *> astarTiles;
        for (auto &t : level->get_map().tiles)
            astarTiles.push_back(&t);
        acko = std::make_unique<AStar<Object *>>(astarTiles);
    }

    void user_init()
    {
        level = &add_level();

        level->set_map({0, 0, 0, 0});
        level->get_map().init(40, 36);

        level->get_map().add_sprite_property(1, 4, {100, 100, 0, 255});
        level->get_map().add_sprite_property(0, 0, {0, 150, 0, 255});
        level->get_map().add_sprite_property(12, 13, {100, 100, 100, 255});
        level->get_map().add_sprite_property(4, 5, {150, 100, 50, 255}, 10);
        level->get_map().add_sprite_property(0, 1, {0, 0, 150, 255});
        level->add_image("map", "data/gfx/Overworld.png");
        level->set_map_image("map");
        level->get_map().load_map("data/start.map", 16, 16);
        level->get_map().load_blocks("data/startBlock2.map", 16, 16);

        if (!textHelp.load_text(*window, "There is no help. You are on your own.", {255, 100, 100, 255}, 64, base::TILESIZEINPUT * 24))
            return;

        init_astar();

        level->add_melee("attack", Melee(5, 1));
        level->get_melee("attack").knockback = base::TILESIZEPHYSICS/2;

        player = level->add_character("player", 9, 9);
        level->add_image("player", "data/gfx/character.png");

        level->set_character_image("player", "player", {0, 250, 0, 255});

        preset::get_animation_data();
        level->add_character_animation("player", "DEFAULT_DOWN", preset::animationData["DEFAULT_DOWN"].get_animation(), "player");
        level->add_character_animation("player", "DEFAULT_RIGHT", preset::animationData["DEFAULT_RIGHT"].get_animation(), "player");
        level->add_character_animation("player", "DEFAULT_LEFT", preset::animationData["DEFAULT_LEFT"].get_animation(), "player");
        level->add_character_animation("player", "DEFAULT_UP", preset::animationData["DEFAULT_UP"].get_animation(), "player");
        level->add_character_animation("player", "WALK_DOWN", preset::animationData["WALK_DOWN"].get_animation(), "player");
        level->add_character_animation("player", "WALK_RIGHT", preset::animationData["WALK_RIGHT"].get_animation(), "player");
        level->add_character_animation("player", "WALK_LEFT", preset::animationData["WALK_LEFT"].get_animation(), "player");
        level->add_character_animation("player", "WALK_UP", preset::animationData["WALK_UP"].get_animation(), "player");
        level->add_character_animation("player", "ATTACK_DOWN", preset::animationData["ATTACK_DOWN"].get_animation(), "player");
        level->add_character_animation("player", "ATTACK_RIGHT", preset::animationData["ATTACK_RIGHT"].get_animation(), "player");
        level->add_character_animation("player", "ATTACK_LEFT", preset::animationData["ATTACK_LEFT"].get_animation(), "player");
        level->add_character_animation("player", "ATTACK_UP", preset::animationData["ATTACK_UP"].get_animation(), "player");
        level->add_image("attack", "data/attack.bmp");
        level->add_character_animation("player", "ATTACK", preset::animationData["ATTACK"].get_animation(), "attack");
        level->set_character_property("player", "DO_ATTACK", 0);
        player->set_health(99);
        level->add_character_melee("player", "attack");

        level->add_character("CH1", 1, 8);
        level->set_character_image("CH1", "player", {255, 0, 0, 255});
        level->copy_character_animation("player", "CH1");
        level->set_character_target("CH1", "player");
        level->set_character_property("CH1", "DO_ATTACK", 0);
        // TODO: preset for health/speed/other...
        //    c0.speed = base::TILESIZE/64;
        level->add_character_melee("CH1", "attack");

        level->add_character("CH2", 2, 8);
        level->set_character_image("CH2", "player", {255, 155, 0, 255});
        level->copy_character_animation("player", "CH2");
        level->set_character_target("CH2", "player");
        level->set_character_property("CH2", "DO_ATTACK", 0);
        level->add_character_melee("CH2", "attack");
        //    c1.speed = base::TILESIZE/32;

        level->add_character("CH3", 7, 7);
        level->set_character_image("CH3", "player", {0, 0, 255, 255});
        level->copy_character_animation("player", "CH3");
        // TODO: target character -> what on death of target?
        level->set_character_target("CH3", "player");
        level->set_character_property("CH3", "DO_ATTACK", 0);
        level->add_character_melee("CH3", "attack");
        //    c2.speed = base::TILESIZE/32;

        menu = &add_menu(Menu::RIGHT, {0, 0, 12*base::TILESIZEINPUT, 0});
        int buttonW = base::TILESIZEINPUT * 4;
        int buttonH = base::TILESIZEINPUT * 2;
        menu->add_image("button", "data/button.bmp");

        menu->add_button("help", {0, 0, buttonW, buttonH});
        menu->set_button_image("help", "button", "HELP");

        menu->add_button("fps", {buttonW, 0, buttonW, buttonH});
        menu->set_button_image("fps", "button", "FPS");

        menu->add_button("pause", {buttonW * 2, 0, buttonW, buttonH}, 2, 1, SDLK_p);
        menu->set_button_image("pause", "button", "pause");

        menu->add_button("+", {0, buttonH, buttonW, buttonH});
        menu->set_button_image("+", "button", "+");

        menu->add_button("-", {buttonW, buttonH, buttonW, buttonH});
        menu->set_button_image("-", "button", "-");

        menu->add_button("quit", {buttonW * 2, buttonH, buttonW, buttonH});
        menu->set_button_image("quit", "button", "EXIT");

    }

    void user_evaluate()
    {

        player = &level->get_char("player");
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
    }

    void user_update()
    {
        if (player->property["health"] == 0)
            quit = true;

        if (menu->get_state("pause"))
            level->pause = !level->pause;
        if (menu->get_state("help"))
            help = !help;
        if (menu->get_state("quit"))
            quit = true;
        if(menu->get_state("fps")) showFPS = !showFPS;
        if (menu->get_state("+") && base::TILESIZERENDER < 6 * base::TILESIZEINPUT)
            base::set_tilerender(base::TILESIZERENDER * 2);
        if (menu->get_state("-") && base::TILESIZERENDER > base::TILESIZEINPUT)
            base::set_tilerender(base::TILESIZERENDER / 2);

        // move stuff (if not paused)
        if (!level->pause)
        {
            for (auto &chrIt : level->get_chars())
            {
                Character *chr = &chrIt.second;
                if (chr == player)
                    continue;
                // use AStar to move non-player chars
                if (AIclick % 50 == 0)
                {
                    auto tmp = acko->find_path(chr, chr->target, level->get_collisionObjects());
                    if (!tmp.empty())
                        chr->path = tmp;
                }
                chr->follow_path(level->get_collisionObjects());
            }
            AIclick++;

            level->move_chars();

            // process player
            if (level->screenClick)
                level->set_character_property("player", "DO_ATTACK", 1);
            else
                level->set_character_property("player", "DO_ATTACK", 0);

            std::string dir = level->get_direction("player");

            custom_process(player, dir);
            // TODO: setting of targets

            for (auto &chrIt : level->get_chars())
            {
                Character *chr = &chrIt.second;
                if (chr == player)
                    continue;
                const std::string &chrName = chrIt.first;

                if (tick(20) && fabs(chr->position.x - chr->target->position.x) < chr->TILESIZEPHYSICS * 2 && fabs(chr->position.y - chr->target->position.y) < chr->TILESIZEPHYSICS * 2)
                    level->set_character_property(chrName, "DO_ATTACK", 1);
                else
                    level->set_character_property(chrName, "DO_ATTACK", 0);

                dir = level->get_direction(chrName);
                custom_process(chr, dir);
            }
        }

        level->get_map().screen_position(level->screenRect, level->viewPort, *player);
    }

    void user_plot()
    {
        // Render within the menu:
        menu->set_viewPort();
        textHealth.str("");
        textHealth << "Health: " << player->property["health"];
        if (!playerHealth.load_text(*window, textHealth.str(), {255, 100, 100, 255}, 32, base::TILESIZEINPUT * 12))
            return;
        playerHealth.render_image(*window, 0, base::TILESIZEINPUT * 4);

        level->set_viewPort();
        if(showFPS)
        {
            streamFPS.str("");
            streamFPS << "FPS: " << currentFPS << "\t Tickrate: " << currentTickrate;
            if (!textFPS.load_text(*window, streamFPS.str(), {255, 100, 100, 255}, 16, base::TILESIZEINPUT * 24))
                return;
            textFPS.render_image(*window, 0, 0);
        }

        if (help)
            textHelp.render_image(*window, 0, 32);
    }

    void custom_process(Character *object, std::string dir)
    {
        if (object->animations["ATTACK_UP"].running ||
            object->animations["ATTACK_DOWN"].running ||
            object->animations["ATTACK_LEFT"].running ||
            object->animations["ATTACK_RIGHT"].running)
        {
        }
        else if (object->property["DO_ATTACK"]
                 // && level->get_melee("attack").evaluate(object, level->get_collisionObjects()) )
                 && object->melees.at("attack").evaluate(object, level->get_collisionObjects()))
        {
            object->set_animation("ATTACK_" + dir);
        }
        else if (object->moved)
        {
            object->set_animation("WALK_" + dir);
        }
        else
        {
            object->set_animation("DEFAULT_" + dir);
        }
    }

    virtual screen_ptr user_nextScreen();
};

#endif