#include "timer.h"

#include "level.h"
#include "preset.h"
#include "menu.h"
#include "player.h"

#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

void top_down_fight(SDL_wrapper& wrapper);

int main( int argc, char* args[] )
{
    SDL_wrapper wrapper;
    wrapper.set_tilerender(64);
    if( !wrapper.init() ) return 1;

    Menu menu(&wrapper, Menu::CENTER, {0,0,0,0});
    int buttonW = wrapper.TILESIZEINPUT*12;
    int buttonH = wrapper.TILESIZEINPUT*4;
    menu.add_image("button", "data/button.bmp");
   
    menu.add_button("topdown", {0,buttonH,buttonW,buttonH});
    menu.set_button_image("topdown", "button", "Top-down fighter", 8);

    menu.add_button("help", {0,buttonH*3,buttonW,buttonH});
    menu.set_button_image("help", "button", "HELP",16);

    menu.add_button("quit", {0,buttonH*5,buttonW,buttonH});
    menu.set_button_image("quit", "button", "EXIT",16);

    bool quit = false;
    bool help = false;
    SDL_Event event;
    //While application is running
    while( !quit )
    {
        menu.reset();

        // process inputs
        while( SDL_PollEvent( &event ) != 0 )
        {
            if( event.type == SDL_QUIT )
            {
                quit = true;
            }
            menu.evaluate(event);
        }

        if(menu.get_state("topdown")) top_down_fight(wrapper);
        if(menu.get_state("help")) help = !help;
        if(menu.get_state("quit")) quit = true;

        wrapper.clear();
        menu.plot();
        wrapper.render();
    }
}

void top_down_fight(SDL_wrapper& wrapper)
{
    Level level(&wrapper);

    enum tiles{ DIRT, GRASS, STONES, WOOD, WATTER };
    level.get_map().add_sprite(DIRT,1,4, {100,100,0,255} );
    level.get_map().add_sprite(GRASS,0,0, {0,150,0,255} );
    level.get_map().add_sprite(STONES,12,13, {100,100,100,255} );
    level.get_map().add_sprite(WOOD,4,5, {150,100,50,255}, 10 );
    level.get_map().add_sprite(WATTER,0,1, {0,0,150,255} );
    level.add_image("map", "data/gfx/Overworld.png");
    level.set_map_image("map");
    level.get_map().load_map("data/start.map",16,16);
    level.get_map().load_blocks("data/startBlock2.map",16,16);
    level.init_astar();

    level.add_melee("attack", Melee(5,1));
    level.get_melee("attack").knockback = base::TILESIZE;

    level.add_image("player", "data/gfx/character.png");
    level.set_player_image("player", {0,250,0,255});
    level.set_player_position(9 ,9);

    preset::get_animation_data();
    level.add_player_animation("DEFAULT_DOWN", preset::animationData["DEFAULT_DOWN"].get_animation(), "player");
    level.add_player_animation("DEFAULT_RIGHT", preset::animationData["DEFAULT_RIGHT"].get_animation(), "player");
    level.add_player_animation("DEFAULT_LEFT", preset::animationData["DEFAULT_LEFT"].get_animation(), "player");
    level.add_player_animation("DEFAULT_UP", preset::animationData["DEFAULT_UP"].get_animation(), "player");
    level.add_player_animation("WALK_DOWN", preset::animationData["WALK_DOWN"].get_animation(), "player");
    level.add_player_animation("WALK_RIGHT", preset::animationData["WALK_RIGHT"].get_animation(), "player");
    level.add_player_animation("WALK_LEFT", preset::animationData["WALK_LEFT"].get_animation(), "player");
    level.add_player_animation("WALK_UP", preset::animationData["WALK_UP"].get_animation(), "player");
    level.add_player_animation("ATTACK_DOWN", preset::animationData["ATTACK_DOWN"].get_animation(), "player");
    level.add_player_animation("ATTACK_RIGHT", preset::animationData["ATTACK_RIGHT"].get_animation(), "player");
    level.add_player_animation("ATTACK_LEFT", preset::animationData["ATTACK_LEFT"].get_animation(), "player");
    level.add_player_animation("ATTACK_UP", preset::animationData["ATTACK_UP"].get_animation(), "player");
    level.add_image("attack", "data/attack.bmp");
    level.add_player_animation("ATTACK", preset::animationData["ATTACK"].get_animation(), "attack");
    level.set_player_property("DO_ATTACK", 0);
    level.get_player()->set_health(99);

    level.add_character("CH1", 1, 8);
    level.set_character_image("CH1", "player", {255,0,0,255});
    level.copy_character_animation("player", "CH1");
    level.set_character_target("CH1", "player");
    level.set_character_property("CH1", "DO_ATTACK", 0);
    // TODO: preset for health/speed/other...
    //    c0.speed = base::TILESIZE/64;

    level.add_character("CH2", 2, 8);
    level.set_character_image("CH2", "player", {255,155,0,255});
    level.copy_character_animation("player", "CH2");
    level.set_character_target("CH2", "player");
    level.set_character_property("CH2", "DO_ATTACK", 0);
    //    c1.speed = base::TILESIZE/32;

    level.add_character("CH3", 7, 7);
    level.set_character_image("CH3", "player", {0,0,255,255});
    level.copy_character_animation("player", "CH3");
    // TODO: target character -> what on death of target?
    level.set_character_target("CH3", "player");
    level.set_character_property("CH3", "DO_ATTACK", 0);
    //    c2.speed = base::TILESIZE/32;

    Menu menu(&wrapper);
    int buttonW = wrapper.TILESIZEINPUT*4;
    int buttonH = wrapper.TILESIZEINPUT*2;
    menu.add_image("button", "data/button.bmp");
    
    menu.add_button("help", {0,0,buttonW,buttonH});
    menu.set_button_image("help", "button", "HELP");

    menu.add_button("fps", {buttonW,0,buttonW,buttonH});
    menu.set_button_image("fps", "button", "FPS");

    menu.add_button("pause", {buttonW*2,0,buttonW,buttonH}, SDLK_p);
    menu.set_button_image("pause", "button", "pause");

    menu.add_button("+", {0,buttonH,buttonW,buttonH});
    menu.set_button_image("+", "button", "+");

    menu.add_button("-", {buttonW,buttonH,buttonW,buttonH});
    menu.set_button_image("-", "button", "-");

    menu.add_button("quit", {buttonW*2,buttonH,buttonW,buttonH});
    menu.set_button_image("quit", "button", "EXIT");

    //Load media
    IMG_wrapper textHelp;
    if( !textHelp.load_text("There is no help. You are on your own.", {0,0,0,255},wrapper.TILESIZERENDER/2, wrapper.sWidth/4) ) return;

    //Main loop flag
    bool quit = false;
    bool help = false;
    SDL_Event event;

    timer FPS;
    FPS.start();
    int nFrames = 0;
    std::stringstream textFPS;
    IMG_wrapper imgFPS;
    bool showFPS = false;

    IMG_wrapper playerHealth;
    std::stringstream textHealth;

    //While application is running
    while( !quit )
    {
        if( level.get_player()->property["health"]==0 ) break;
        // TODO: naming! preprocess vs reset()!
        level.preprocess();
        menu.reset();

        // process inputs
        while( SDL_PollEvent( &event ) != 0 )
        {
            if( event.type == SDL_QUIT )
            {
                quit = true;
            }
            level.evaluate(event);
            menu.evaluate(event);
        }

        if(menu.get_state("pause")) level.pause = !level.pause;
        if(menu.get_state("help")) help = !help;
        if(menu.get_state("quit")) quit = true;
        if(menu.get_state("fps")) showFPS = !showFPS;
        if(menu.get_state("+")
           && wrapper.TILESIZERENDER < 6*wrapper.TILESIZEINPUT
        ) wrapper.set_tilerender(wrapper.TILESIZERENDER * 2);
        if(menu.get_state("-")
           && wrapper.TILESIZERENDER > wrapper.TILESIZEINPUT
        ) wrapper.set_tilerender(wrapper.TILESIZERENDER / 2);

        // move stuff (if not paused)
        if(!level.pause)
        {
            Player*  player = level.get_player();
            level.process();                

            if(level.screenClick) 
                level.set_player_property("DO_ATTACK", 1);
            else
                level.set_player_property("DO_ATTACK", 0);


            std::string dir = level.get_direction("player");

            auto custom_process = [&level, &dir](Movable *object)
            {
                if (object->animations["ATTACK_UP"].running ||
                    object->animations["ATTACK_DOWN"].running ||
                    object->animations["ATTACK_LEFT"].running ||
                    object->animations["ATTACK_RIGHT"].running
                   )
                {
                }
                else if (object->property["DO_ATTACK"] 
                    && level.get_melee("attack").evaluate(object, level.get_collisionObjects()) )
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
            };
            custom_process(player);
            // TODO: setting of targets

            for( auto& chrIt : level.get_chars() )
            {
                Character* chr = &chrIt.second;
                const std::string& chrName = chrIt.first;
                
                if(level.tick(20) && fabs(chr->position.x-chr->target->position.x)<chr->TILESIZE*2 && fabs(chr->position.y-chr->target->position.y)<chr->TILESIZE*2)
                    level.set_character_property(chrName, "DO_ATTACK", 1);
                else level.set_character_property(chrName, "DO_ATTACK", 0);

                dir  = level.get_direction(chrName);
                custom_process(chr);
            }
        }

        wrapper.clear();
        level.plot();

        double avgFPS;
        if( showFPS && nFrames%60 == 0)
        {
            //Calculate and correct fps
            avgFPS = nFrames / ( FPS.getTicks() / 1000.f );
            if( avgFPS > 2000000 )
            {
                avgFPS = 0;
            }
            textFPS.str("");
            textFPS << avgFPS;
            if( !imgFPS.load_text(textFPS.str(), {100,255,100,255}) ) return;
            FPS.restart();
            nFrames=0;
        }
        if(showFPS)
        {
            wrapper.render_image(imgFPS,0,0);
            ++nFrames;
        }
        if(help) wrapper.render_image(textHelp,0,wrapper.sHeight/4+wrapper.TILESIZERENDER/2 );

        menu.plot();
        textHealth.str("");
        textHealth <<"Health: " << level.get_player()->property["health"];
        if( !playerHealth.load_text(textHealth.str(), {100,255,100,255},32,wrapper.TILESIZEINPUT*12) ) return;
        wrapper.render_image(playerHealth,0,wrapper.TILESIZEINPUT*4);

        wrapper.render();

    }

    return;
}
