#ifndef TOPDOWN_H
#define TOPDOWN_H

#include <algorithm>

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
    AI<Block*> ai;
    std::map<std::string, std::unique_ptr<Damager>> dmgrs;
    std::map<std::string, Animation> anims;
    std::map<std::string, SDL_Rect> animPoss;
    std::map<std::string, Dmgr_instance> dmgr_insts;

    void user_init()
    {
        level = &add_level();

        level->set_map({0, 0, 0, 0});
        level->get_map().init(40, 36);

        level->get_map().add_sprite_property(1, 4, {100, 100, 0, 255});
        level->get_map().add_sprite_property(0, 0, {0, 150, 0, 255});
        level->get_map().add_sprite_property(12, 13, {100, 100, 100, 255});
        level->get_map().add_sprite_property(4, 5, {150, 100, 50, 255});
        level->get_map().add_sprite_property(0, 1, {0, 0, 150, 255});
        level->add_image("map", "data/gfx/Overworld.png");
        level->set_map_image("map");
        level->get_map().load_map("data/start.map", 16, 16);
        level->get_map().load_blocks("data/startBlock2.map", 16, 16);

        if (!textHelp.load_text(*window, "There is no help. You are on your own.", {255, 100, 100, 255}, 64, base::TILESIZEINPUT * 24))
            return;

        std::vector<Block *> astarTiles;
        for (auto &t : level->get_map().tiles)
            astarTiles.push_back(&t);
        ai.init_astar(astarTiles);

        dmgrs.emplace("sword", std::make_unique<AreaDamager>(5, 1));
        dmgrs.at("sword")->knockback = 32;
        dmgrs.emplace("explosion", std::make_unique<AreaDamager>(10, 0, 3, 3));
        dmgrs.at("explosion")->knockback = 128;
        dmgrs.emplace("gun", std::make_unique<HitScanDamager>(5, 5, 0.5));
        dmgrs.at("gun")->knockback = 32;
        dmgrs.at("gun")->cooldown = 0.15;

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
        level->add_image("explosion", "data/expl/wills_pixel_explosions_sample/round_explosion/spritesheet/spritesheet.png");
        level->add_character_animation("player", "ATTACK", preset::animationData["ATTACK"].get_animation(), "attack");
        anims.emplace("explosion", preset::animationData["EXPLOSION"].get_animation());
        anims.at("explosion").image = level->get_image("explosion");
        player->set_health(999);
        player->dmgr_insts.emplace("sword", Dmgr_instance(dmgrs.at("sword").get()));
        player->dmgr_insts.emplace("explosion", Dmgr_instance(dmgrs.at("explosion").get()));
        player->dmgr_insts.emplace("gun", Dmgr_instance(dmgrs.at("gun").get()));

        auto chr = level->add_character("CH1", 1, 8);
        level->set_character_image("CH1", "player", {255, 0, 0, 255});
        level->copy_character_animation("player", "CH1");
        //level->set_character_target("CH1", "player");
        // TODO: preset for health/speed/other...
        //    c0.speed = base::TILESIZE/64;
        chr->dmgr_insts.emplace("sword", Dmgr_instance(dmgrs.at("sword").get()));

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
            case SDLK_w:
                player->move_up(level->get_collisionObjects());
                break;
            case SDLK_s:
                player->move_down();
                break;
            case SDLK_a:
                player->move_left();
                break;
            case SDLK_d:
                player->move_right();
                break;
            case SDLK_q:
                player->dmgr_insts.at("explosion").doAtack = true;
                break;
            case SDLK_e:
                player->dmgr_insts.at("gun").doAtack = true;
                break;
            }
        }
        else if (event.type == SDL_KEYUP && event.key.repeat == 0)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                player->move_down();
                break;
            case SDLK_s:
                player->move_up(level->get_collisionObjects());
                break;
            case SDLK_a:
                player->move_right();
                break;
            case SDLK_d:
                player->move_left();
                break;
            case SDLK_q:
                player->dmgr_insts.at("explosion").doAtack = false;
                break;
            case SDLK_e:
                player->dmgr_insts.at("gun").doAtack = false;
                break;
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
            ai.increment();
            // Setup path for NPCs
            for (auto &chrIt : level->get_chars())
            {
                Character *chr = &chrIt.second;
                if (chr == player || !chr->target)
                    continue;
                // use AStar to move non-player chars
                if (ai.tick(50))
                {
                    auto tmp = ai.acko->find_path(chr, chr->target, level->get_collisionObjects());
                    if (!tmp.empty())
                        chr->path = tmp;
                }
                chr->follow_path(level->get_collisionObjects());
            }

            level->move_chars();

            // Process player attack decision
            player->dmgr_insts.at("sword").doAtack = level->screenClick;

            // Process NPC attack decision
            for (auto &chrIt : level->get_chars())
            {
                Character *chr = &chrIt.second;
                if (chr == player || !chr->target)
                    continue;

                chr->dmgr_insts.at("sword").doAtack = (ai.tick(20)
                    && fabs(chr->hitbox.x - chr->target->hitbox.x) < chr->TILESIZEPHYSICS * 2
                    && fabs(chr->hitbox.y - chr->target->hitbox.y) < chr->TILESIZEPHYSICS * 2
                );
            }

            // Process attack and animation
            for (auto &chrIt : level->get_chars())
            {
                const std::string& dir = level->get_direction(chrIt.first);
                custom_process(&chrIt.second, dir);
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
        if(anims.at("explosion").running)
            anims.at("explosion").run_and_plot(*window, animPoss["explosion"]);

        // Tracers
        for (auto &chrIt : level->get_chars())
        {
            if(chrIt.second.dmgr_insts.count("gun")==0) continue;
            auto& gun = chrIt.second.dmgr_insts.at("gun");
            HitScanDamager* hs = dynamic_cast<HitScanDamager*>(gun.dmgr);
            float cd = gun.get_cooldown_fraction();
            if(cd < 0.5)
            {
                float shift = std::max(0., cd/0.5-0.05);
                auto start = tools::get_endpoint(hs->origin, hs->endpoint, hs->range*base::TILESIZEPHYSICS*shift);
                auto end = tools::get_endpoint(start, hs->endpoint, hs->range*base::TILESIZEPHYSICS*(0.1));
                window->drawColorLine(
                    base::toScreen(&level->screenRect, start),
                    base::toScreen(&level->screenRect, end),
                    {255, 255, 255, 150}
                );
            }
        }
    }

    void custom_process(Character *object, std::string dir)
    {
        if (object->evaluate_attack("sword", level->get_damagableObjects()))
        {
            // stop any previous running animation
            object->get_current_animation().stop();
            object->set_animation("ATTACK_" + dir);
        }
        else if (object->evaluate_attack("explosion",
                     base::fromScreen(&level->screenRect, level->mousePositionScreen),
                     Object::direction::DOWN, level->get_damagableObjects())
        )
        {
            anims.at("explosion").play();
            animPoss["explosion"] = level->mousePositionScreen;
        }
        else if (object->evaluate_attack("gun",
                     object->position(), base::fromScreen(&level->screenRect, level->mousePositionScreen),
                     level->get_damagableObjects())
        )
        {
        }
        else if (
            tools::contains(object->get_current_animation_name(), "ATTACK_")
            && object->get_current_animation().running)
        {
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