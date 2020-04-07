#include "SDL_wrapper.h"
#include "player.h"
#include "button.h"
#include "object.h"
#include "timer.h"
#include "map.h"
#include "astar.h"

#include <iostream>
#include <sstream>
#include <cmath>

SDL_Renderer * IMG_wrapper::gRenderer(nullptr);
int base::mWidth(0);
int base::mHeight(0);
int base::sWidth(0);
int base::sHeight(0);

int main( int argc, char* args[] )
{
    SDL_wrapper wrapper;

    enum tiles{ DIRT, GRASS, STONES, WOOD, WATTER };

    //Start up SDL and create window
    if( wrapper.init() )
    {
        IMG_wrapper::gRenderer = wrapper.gRenderer;
        M2DI::Map_wrapper curMap;
        curMap.load_map("data/start.map",16,16);
        if( !curMap.image->load_media("data/maptiles.bmp") ) return 1;

        curMap.add_sprite(DIRT,0,0, {100,100,0,255} );
        curMap.add_sprite(GRASS,1,0, {0,150,0,255} );
        curMap.add_sprite(STONES,2,0, {100,100,100,255} );
        curMap.add_sprite(WOOD,3,0, {150,100,50,255} );
        curMap.add_sprite(WATTER,4,0, {0,0,150,255} );

        //Render red filled quad

        std::shared_ptr<player> p = std::make_shared<player>(wrapper.TILESIZERENDER*4,wrapper.TILESIZERENDER*8);
        if(!p->set_image("data/player2.bmp") ) return 1;
        p->mapColor = {0,250,0,255};

        std::shared_ptr<character> c = std::make_shared<character>(wrapper.TILESIZERENDER*1,wrapper.TILESIZERENDER*8,1);
        if( !c->set_image("data/player2.bmp") ) return 1;
        c->mapColor = {255,0,0,255};
        c->speed = c->TILESIZERENDER/32;

        button b1(0,wrapper.sHeight/2);
        if(!b1.set_image("data/button.bmp","HELP")) return 1;
        button b2(wrapper.TILESIZERENDER/2,wrapper.sHeight/2);
        if(!b2.set_image("data/button.bmp","FPS") ) return 1;

        //Load media
        // TODO: simplify somehow so render does nto have to be passed?
        IMG_wrapper textHelp;
        if( !textHelp.load_text("There is no help. You are on your own.", {0,0,0,255},wrapper.TILESIZERENDER/2, wrapper.sWidth/4) ) return 1;

        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;
        SDL_Rect screenRect;
        int sX = 0, sY =0;
        bool help = false;

        timer FPS;
        FPS.start();
        int nFrames = 0;
        std::stringstream textFPS;
        IMG_wrapper imgFPS;
        bool showFPS = false;

        std::vector<std::shared_ptr<object>> collObjects;
        collObjects.push_back(c);
        collObjects.push_back(p);
        for( auto t : curMap.tiles )
        {
            if( t->spriteType == WATTER ) collObjects.push_back(t);
        }

        int AIclick = 0;
        astar acko(curMap.tiles);
        std::vector<std::shared_ptr<object>> path = {};
        float dirX = p->position.x - c->position.x;
        float dirY = p->position.y - c->position.y;

        //While application is running
        while( !quit )
        {
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
                p->evaluate(e);
                if(b1.evaluate(e) == b1.CLICK) help =  !help;
                if(b2.evaluate(e) == b2.CLICK) showFPS = !showFPS;
            }
            //p->evaluate( SDL_GetKeyboardState( NULL ) );
            SDL_RenderClear( wrapper.gRenderer );

            p->move(collObjects);
            p->screen_position(sX,sY);
            screenRect = {sX,sY,wrapper.sWidth,wrapper.sHeight};
            curMap.render_map( wrapper, screenRect );

            if( AIclick%50==0 )
            {
                path = acko.find_path(c,p,collObjects);
                if(path.empty()) std::cout << "empty path" << std::endl;
            }
            if( !path.empty() )
            {
                for( auto t : path )
                {
                    t->image->set_color( 250, 0, 0 );
                    t->plot(wrapper,&screenRect);
                    t->image->set_color( 255, 255, 255 );
                }
                if(  SDL_HasIntersection(&c->position,&path.back()->position ) ) path.pop_back();
                dirX = path.back()->position.x - c->position.x;
                dirY = path.back()->position.y - c->position.y;
                if( dirX > 0 ) c->velX = c->speed;
                else if( dirX < 0 ) c->velX = -c->speed;
                if( dirY > 0 ) c->velY = c->speed;
                else if( dirY < 0 ) c->velY = -c->speed;
            } else std::cout << "empty path" << std::endl;
            AIclick++;

            c->move(collObjects);

            p->plot(wrapper);
            c->plot(wrapper,&screenRect);

            b1.plot(wrapper);
            if(help) wrapper.render_image(textHelp,0,wrapper.sHeight/4+wrapper.TILESIZERENDER/2 );
            b2.plot(wrapper);

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
                wrapper.render_image(imgFPS,wrapper.sWidth-wrapper.TILESIZERENDER,0);
                ++nFrames;
            }

            curMap.render_minimap(wrapper,collObjects);
            SDL_SetRenderDrawColor( wrapper.gRenderer, 0x00, 0x00, 0x00, 0xFF );
            SDL_RenderPresent( wrapper.gRenderer );
        }
    }

    return 0;
}
