#include "SDL_wrapper.h"
#include "player.h"
#include "button.h"
#include "object.h"
#include "timer.h"
#include "map.h"
#include "astar.h"
#include "melee.h"

#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>

SDL_Renderer * IMG_wrapper::gRenderer(nullptr);
int base::mWidth(0);
int base::mHeight(0);
int base::sWidth(0);
int base::sHeight(0);
int base::TILESIZERENDER(0);
double base::scaleRender;
double base::scaleRenderInput;
SDL_Rect gameplay::gameplayScreen={0,0,0,0};

template<typename T>
void remove_dead( std::vector<T> & objects )
{
    //std::vector<Object> toRemove;
    //for( auto o : objects ) if( o->property.count("health") && o->property["health"]==0 ) toRemove.push_back(o);
    //#for( auto tR: toRemove ) objects.erase( std::find(objects.begin(), objects.end(), tR) );
    objects.erase(std::remove_if(objects.begin(), objects.end(),
                  [](const T& o)
                  { return o->property.count("health") && o->property.at("health")==0; }),
                  objects.end());
}

int main( int argc, char* args[] )
{
    SDL_wrapper wrapper;
    wrapper.set_tilerender(64);

    enum tiles{ DIRT, GRASS, STONES, WOOD, WATTER };


    //Start up SDL and create window
    if( wrapper.init() )
    {
        IMG_wrapper::gRenderer = wrapper.gRenderer;
        M2DI::Map_wrapper curMap;
        curMap.add_sprite(DIRT,0,0, {100,100,0,255} );
        curMap.add_sprite(GRASS,1,0, {0,150,0,255} );
        curMap.add_sprite(STONES,2,0, {100,100,100,255} );
        curMap.add_sprite(WOOD,3,0, {150,100,50,255}, 10 );
        curMap.add_sprite(WATTER,0,1, {0,0,150,255} );
        if( !curMap.image->load_media("data/maptiles3.bmp") ) return 1;
        curMap.load_map("data/start.map",16,16);
        curMap.load_blocks("data/startBlock.map",16,16);

        // Astar needs the same scope as the map!
        std::vector<Object*> astarTiles;
        for( auto& t : curMap.tiles ) astarTiles.push_back(t.get());
        AStar<Object*> acko(astarTiles);

        Melee attack(5, 1);
        attack.knockback = base::TILESIZE;

        Melee trap(3, 0);
        trap.knockback = base::TILESIZE;

        Player player = Player(wrapper.TILESIZE*9,wrapper.TILESIZE*9);
        if(!player.set_image("data/player3.bmp") ) return 1;
        player.mapColor = {0,250,0,255};
        player.animations["WALK"] = std::make_shared<animation>(0,player.TILESIZE/2);
        player.animations["WALK"]->clips.push_back({player.TILESIZEINPUT*0,player.TILESIZEINPUT*2,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALK"]->clips.push_back({player.TILESIZEINPUT*1,player.TILESIZEINPUT*2,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALK"]->clips.push_back({player.TILESIZEINPUT*2,player.TILESIZEINPUT*2,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALK"]->clips.push_back({player.TILESIZEINPUT*1,player.TILESIZEINPUT*2,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALK"]->clips.push_back({player.TILESIZEINPUT*0,player.TILESIZEINPUT*2,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALK"]->clips.push_back({player.TILESIZEINPUT*1,player.TILESIZEINPUT*5/2,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALK"]->clips.push_back({player.TILESIZEINPUT*2,player.TILESIZEINPUT*5/2,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALK"]->clips.push_back({player.TILESIZEINPUT*1,player.TILESIZEINPUT*5/2,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALK"]->image = player.image;
        player.animations["WALKUPPER"] = std::make_shared<animation>(0,player.TILESIZE/4);
        player.animations["WALKUPPER"]->clips.push_back({player.TILESIZEINPUT*0,player.TILESIZEINPUT*1,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALKUPPER"]->clips.push_back({player.TILESIZEINPUT*1,player.TILESIZEINPUT*3/2,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALKUPPER"]->clips.push_back({player.TILESIZEINPUT*2,player.TILESIZEINPUT*3/2,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALKUPPER"]->clips.push_back({player.TILESIZEINPUT*1,player.TILESIZEINPUT*3/2,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALKUPPER"]->clips.push_back({player.TILESIZEINPUT*0,player.TILESIZEINPUT*1,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALKUPPER"]->clips.push_back({player.TILESIZEINPUT*1,player.TILESIZEINPUT*1,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALKUPPER"]->clips.push_back({player.TILESIZEINPUT*2,player.TILESIZEINPUT*1,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALKUPPER"]->clips.push_back({player.TILESIZEINPUT*1,player.TILESIZEINPUT*1,player.TILESIZEINPUT*1,player.TILESIZEINPUT/2});
        player.animations["WALKUPPER"]->image = player.image;
        player.animations["HEAD"] = std::make_shared<animation>(0,-player.TILESIZE/2);
        player.animations["HEAD"]->clips.push_back({player.TILESIZEINPUT*0,player.TILESIZEINPUT*0,player.TILESIZEINPUT*1,player.TILESIZEINPUT*1});
        player.animations["HEAD"]->image = player.image;

        // attack, has animation and property which detetermines if it is performed
        player.property["DO_ATTACK"] = 0;
        player.animations["ATTACK"] = std::make_shared<animation>(0,-player.TILESIZE*3/4);
        player.animations["ATTACK"]->clips.push_back({player.TILESIZEINPUT*0,player.TILESIZEINPUT*0,player.TILESIZEINPUT*1,player.TILESIZEINPUT*3});
        player.animations["ATTACK"]->clips.push_back({player.TILESIZEINPUT*1,player.TILESIZEINPUT*0,player.TILESIZEINPUT*1,player.TILESIZEINPUT*3});
        player.animations["ATTACK"]->clips.push_back({player.TILESIZEINPUT*2,player.TILESIZEINPUT*0,player.TILESIZEINPUT*1,player.TILESIZEINPUT*3});
        player.animations["ATTACK"]->clips.push_back({player.TILESIZEINPUT*3,player.TILESIZEINPUT*0,player.TILESIZEINPUT*1,player.TILESIZEINPUT*3});
        player.animations["ATTACK"]->display = false;
        if(!player.animations["ATTACK"]->set_image("data/attack.bmp") ) return 1;


        // TODO: shared animation to unique
        std::vector<std::unique_ptr<Character>> chars;
        chars.push_back(std::make_unique<Character>(wrapper.TILESIZE*1,wrapper.TILESIZE*8));
        Character &c0 = *chars.back();
        c0.image = player.image;
        c0.animations["WALK"] = std::make_shared<animation>(*player.animations["WALK"]);
        c0.animations["WALKUPPER"] = std::make_shared<animation>(*player.animations["WALKUPPER"]);
        c0.animations["HEAD"] = std::make_shared<animation>(*player.animations["HEAD"]);
        c0.animations["ATTACK"] = std::make_shared<animation>(*player.animations["ATTACK"]);
        c0.mapColor = {255,0,0,255};
        c0.speed = base::TILESIZE/64;
        c0.target = &player;

        chars.push_back(std::make_unique<Character>(wrapper.TILESIZE*2,wrapper.TILESIZE*8));
        Character &c1 = *chars.back();
        c1.image = c0.image;
        c1.animations["WALK"] = std::make_shared<animation>(*player.animations["WALK"]);
        c1.animations["WALKUPPER"] = std::make_shared<animation>(*player.animations["WALKUPPER"]);
        c1.animations["HEAD"] = std::make_shared<animation>(*player.animations["HEAD"]);
        c1.animations["ATTACK"] = std::make_shared<animation>(*player.animations["ATTACK"]);
        c1.mapColor = {255,155,0,255};
        c1.speed = base::TILESIZE/32;
        c1.target = &player;

        chars.push_back(std::make_unique<Character>(wrapper.TILESIZE*7,wrapper.TILESIZE*7));
        Character &c2 = *chars.back();
        c2.image = c0.image;
        c2.animations["WALK"] = std::make_shared<animation>(*player.animations["WALK"]);
        c2.animations["WALKUPPER"] = std::make_shared<animation>(*player.animations["WALKUPPER"]);
        c2.animations["HEAD"] = std::make_shared<animation>(*player.animations["HEAD"]);
        c2.animations["ATTACK"] = std::make_shared<animation>(*player.animations["ATTACK"]);
        c2.mapColor = {0,0,255,255};
        c2.speed = base::TILESIZE/32;
        c2.target = &c1;


        int buttonX = 0;// wrapper.sWidth-wrapper.TILESIZEINPUT*6;//curMap.gameplayScreen.w;
        //  std::cout << wrapper.sWidth << " " << wrapper.TILESIZEINPUT*6 << std::endl;
        int buttonY = 0; //wrapper.TILESIZEINPUT*6;
        int buttonW = wrapper.TILESIZEINPUT*4;
        int buttonH = wrapper.TILESIZEINPUT*2;

        SDL_Rect menuVP = {wrapper.sWidth-wrapper.TILESIZEINPUT*12,wrapper.TILESIZEINPUT*12,wrapper.TILESIZEINPUT*12,wrapper.TILESIZEINPUT*6};

        button b1({buttonX,buttonY,buttonW,buttonH});
        b1.viewPort = &menuVP;
        if(!b1.set_image("data/button.bmp","HELP")) return 1;
        button b2({buttonX+buttonW,buttonY,buttonW,buttonH});
        b2.viewPort = &menuVP;
        if(!b2.set_image("data/button.bmp","FPS") ) return 1;
        button b3({buttonX+buttonW*2,buttonY,buttonW,buttonH});
        b3.viewPort = &menuVP;
        if(!b3.set_image("data/button.bmp","pause") ) return 1;
        button b4({buttonX+buttonW,buttonY+buttonH,buttonW,buttonH});
        b4.viewPort = &menuVP;
        if(!b4.set_image("data/button.bmp","+") ) return 1;
        button b5({buttonX+buttonW*2,buttonY+buttonH,buttonW,buttonH});
        b5.viewPort = &menuVP;
        if(!b5.set_image("data/button.bmp","-") ) return 1;
        button b6({buttonX,buttonY+buttonH,buttonW,buttonH});
        b6.viewPort = &menuVP;
        if(!b6.set_image("data/button.bmp","EXIT") ) return 1;

        // button represeting the gameplay screen to evaluate mouse commands
        button bScreen({0,0,0,0});
        bScreen.viewPort = &curMap.gameplayScreen;

        //Load media
        IMG_wrapper textHelp;
        if( !textHelp.load_text("There is no help. You are on your own.", {0,0,0,255},wrapper.TILESIZERENDER/2, wrapper.sWidth/4) ) return 1;

        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;
        SDL_Rect screenRect;
        bool help = false;
        bool pause = false;

        timer FPS;
        FPS.start();
        int nFrames = 0;
        std::stringstream textFPS;
        IMG_wrapper imgFPS;
        bool showFPS = false;

        IMG_wrapper playerHealth;
        std::stringstream textHealth;
        player.set_health(99);
        for( auto& c : chars )
        {
            c->set_health(10);
        }
        std::vector<Object*> collObjects;

        int AIclick = 0;

        //While application is running
        while( !quit )
        {
            remove_dead(chars);
            remove_dead(curMap.blocks);
            if( player.property["health"]==0 ) quit = true;
            collObjects.clear();
            for( auto& t : curMap.blocks ) collObjects.push_back(t.get());
            for( auto& t : curMap.tiles )
            {
                if( t->spriteType == WATTER ) collObjects.push_back(t.get());
            }
            for( auto& c : chars )
            {
                collObjects.push_back(c.get());
            }
            collObjects.push_back(&player);

            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                if( e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED )
                {
                    wrapper.sWidth = e.window.data1;
                    wrapper.sHeight = e.window.data2;
                }
                if(b3.evaluate(e) == b3.CLICK || key_down(e, SDLK_p)) pause = !pause;
                if(b4.evaluate(e) == b4.CLICK || key_down(e, SDLK_PLUS))
                {
                    if(wrapper.TILESIZERENDER<6*wrapper.TILESIZEINPUT)
                        wrapper.set_tilerender(wrapper.TILESIZERENDER*2);
                }
                if(b5.evaluate(e) == b5.CLICK || key_down(e, SDLK_MINUS))
                {
                    if(wrapper.TILESIZERENDER>wrapper.TILESIZEINPUT)
                        wrapper.set_tilerender(wrapper.TILESIZERENDER/2);
                }
                if(b1.evaluate(e) == b1.CLICK || key_down(e, SDLK_h)) help =  !help;
                if(b6.evaluate(e) == b6.CLICK || key_down(e, SDLK_ESCAPE)) quit = true;
                if(b2.evaluate(e) == b2.CLICK || key_down(e, SDLK_s)) showFPS = !showFPS;
                bScreen.screenPos = curMap.gameplayScreen;
                
                // 
                player.evaluate(e);
                if(bScreen.evaluate(e) == bScreen.CLICK) player.property["DO_ATTACK"] = 1;
                else player.property["DO_ATTACK"] = 0;
            }
            //p->evaluate( SDL_GetKeyboardState( NULL ) );

            if(!pause)
            {
                player.move(collObjects);
                if( player.moved )
                {
                    player.animations["WALK"]->play();
                    player.animations["WALKUPPER"]->play();
                }
                else
                {
                    player.animations["WALK"]->stop();
                    player.animations["WALKUPPER"]->stop();
                }
                if(player.property["DO_ATTACK"] && !player.animations["ATTACK"]->running ) // for now cooldown equivalent to animation time
                {
                    attack.evaluate(&player, collObjects, &wrapper);
                    if(player.dir==player.LEFT)
                    {
                        player.animations["ATTACK"]->angle = 0;
                        player.animations["ATTACK"]->flip  = SDL_FLIP_NONE;
                        player.animations["ATTACK"]->shiftX= -player.TILESIZE;
                        player.animations["ATTACK"]->shiftY= -player.TILESIZE;
                    }
                    else if(player.dir==player.RIGHT)
                    {
                        player.animations["ATTACK"]->angle = 0;
                        player.animations["ATTACK"]->flip  = SDL_FLIP_HORIZONTAL;
                        player.animations["ATTACK"]->shiftX= +player.TILESIZE;
                        player.animations["ATTACK"]->shiftY= -player.TILESIZE;
                    }
                    else if(player.dir==player.UP)
                    {
                        player.animations["ATTACK"]->angle = 90;
                        player.animations["ATTACK"]->flip  = SDL_FLIP_NONE;
                        player.animations["ATTACK"]->shiftX= 0;
                        player.animations["ATTACK"]->shiftY= -2*player.TILESIZE;
                    }
                    else if(player.dir==player.DOWN)
                    {
                        player.animations["ATTACK"]->angle = 270;
                        player.animations["ATTACK"]->flip  = SDL_FLIP_NONE;
                        player.animations["ATTACK"]->shiftX= 0;
                        player.animations["ATTACK"]->shiftY= 0;
                    }
                    player.animations["ATTACK"]->play();
                }

                if( AIclick%50==0 ) c2.target = chars.front().get();
                if( c2.target == &c2 ) c2.target = &player;
                for( auto& chr : chars )
                {
                    if( AIclick%50==0 )
                    {
                        auto tmp = acko.find_path(chr.get() ,chr->target,collObjects);
                        if(!tmp.empty()) chr->path = tmp;
                    }
                    chr->follow_path();
                    chr->move(collObjects);
                    if( chr->moved )
                    {
                        chr->animations["WALK"]->play();
                        chr->animations["WALKUPPER"]->play();
                    }
                    else
                    {
                        chr->animations["WALK"]->stop();
                        chr->animations["WALKUPPER"]->stop();
                    }
                    // We don't want alwyas instant reaction so evaluate every 20 frames, check distance and that attack is not already in progress
                    if(  AIclick%20==0 && fabs(chr->position.x-chr->target->position.x)<chr->TILESIZE*2 && fabs(chr->position.y-chr->target->position.y)<chr->TILESIZE*2 && !chr->animations["ATTACK"]->running )
                    {
                        attack.evaluate(chr.get(),collObjects, &wrapper);
                        if(chr->dir==chr->LEFT)
                        {
                            chr->animations["ATTACK"]->angle = 0;
                            chr->animations["ATTACK"]->flip  = SDL_FLIP_NONE;
                            chr->animations["ATTACK"]->shiftX= -player.TILESIZE;
                            chr->animations["ATTACK"]->shiftY= -player.TILESIZE;
                        }
                        else if(chr->dir==chr->RIGHT)
                        {
                            chr->animations["ATTACK"]->angle = 0;
                            chr->animations["ATTACK"]->flip  = SDL_FLIP_HORIZONTAL;
                            chr->animations["ATTACK"]->shiftX= +player.TILESIZE;
                            chr->animations["ATTACK"]->shiftY= -player.TILESIZE;
                        }
                        else if(chr->dir==chr->UP)
                        {
                            chr->animations["ATTACK"]->angle = 90;
                            chr->animations["ATTACK"]->flip  = SDL_FLIP_NONE;
                            chr->animations["ATTACK"]->shiftX= 0;
                            chr->animations["ATTACK"]->shiftY= -2*player.TILESIZE;
                        }
                        else if(chr->dir==chr->DOWN)
                        {
                            chr->animations["ATTACK"]->angle = 270;
                            chr->animations["ATTACK"]->flip  = SDL_FLIP_NONE;
                            chr->animations["ATTACK"]->shiftX= 0;
                            chr->animations["ATTACK"]->shiftY= 0;
                        }
                        chr->animations["ATTACK"]->play();
                    }
                }
                AIclick++;
            }
            SDL_RenderClear( wrapper.gRenderer );
            player.screen_position(screenRect);
            curMap.render_map( wrapper, screenRect );

            for( auto& c : chars) c->plot_path(wrapper,&screenRect);
            //for( auto& c : chars) c->plot_animation(wrapper,&screenRect, pause);
            //player.plot_animation(wrapper, nullptr, pause);
            for(auto & o : collObjects) o->plot_animation(wrapper,&screenRect, pause);

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
                if( !imgFPS.load_text(textFPS.str(), {100,255,100,255}) ) return 1;
                FPS.restart();
                nFrames=0;
            }
            if(showFPS)
            {
                wrapper.render_image(imgFPS,0,0);
                ++nFrames;
            }
            if(help) wrapper.render_image(textHelp,0,wrapper.sHeight/4+wrapper.TILESIZERENDER/2 );

            curMap.render_minimap(wrapper,collObjects);

            // do we need to do this every time?
            menuVP = {wrapper.sWidth-wrapper.TILESIZEINPUT*12,wrapper.TILESIZEINPUT*12,wrapper.TILESIZEINPUT*12,wrapper.TILESIZEINPUT*6};
            SDL_RenderSetViewport( wrapper.gRenderer, &menuVP );
            b1.plot(wrapper);
            b2.plot(wrapper);
            b3.plot(wrapper);
            b4.plot(wrapper);
            b5.plot(wrapper);
            b6.plot(wrapper);
            textHealth.str("");
            textHealth <<"Health: " << player.property["health"];
            if( !playerHealth.load_text(textHealth.str(), {100,255,100,255},32,wrapper.TILESIZEINPUT*12) ) return 1;
            wrapper.render_image(playerHealth,0,wrapper.TILESIZEINPUT*4);

            SDL_SetRenderDrawColor( wrapper.gRenderer, 0x00, 0x00, 0x00, 0xFF );
            SDL_RenderPresent( wrapper.gRenderer );
        }
    }

    return 0;
}
