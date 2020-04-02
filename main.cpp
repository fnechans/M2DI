#include "SDL_wrapper.h"
#include "player.h"
#include "button.h"
#include "object.h"
#include "timer.h"
#include "map.h"

#include <iostream>
#include <sstream>

SDL_Renderer * IMG_wrapper::gRenderer(nullptr);

int main( int argc, char* args[] )
{
    SDL_wrapper wrapper;


    //Start up SDL and create window
    if( wrapper.init() )
    {
        IMG_wrapper::gRenderer = wrapper.gRenderer;

        M2DI::map minimap( (SDL_Rect) {0,0,wrapper.sWidth/4,wrapper.sHeight/4} );
        SDL_Rect screenWP  = {0,0,wrapper.sWidth,wrapper.sHeight};

        //Render red filled quad

        std::shared_ptr<player> p = std::make_shared<player>();
        if(!p->set_image("data/player2.bmp") ) return 1;
        p->mapColor = {0,0,255,255};

        std::shared_ptr<object> c = std::make_shared<object>(wrapper.mWidth/2+234,wrapper.mHeight/2-324,1);
        if( !c->set_image("data/player2.bmp") ) return 1;

        button b1(0,wrapper.sHeight/4, b1.TILESIZE, b1.TILESIZE/2);
        if(!b1.set_image("data/button.bmp","HELP")) return 1;
        button b2(32,wrapper.sHeight/4, b1.TILESIZE, b1.TILESIZE/2);
        if(!b2.set_image("data/button.bmp","FPS") ) return 1;

        //Load media
        // TODO: simplify somehow so render does nto have to be passed?
        IMG_wrapper img1, textHelp;
        if( !img1.load_media("data/map2.bmp") ) return 1;
        if( !textHelp.load_text("There is no help. You are on your own.", {0,0,0,255},wrapper.TILESIZE/2, wrapper.sWidth/4) ) return 1;

        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;
        SDL_Rect screenRect;
        const Uint8* currentKeyStates;
        int sX = 0, sY =0;
        bool help = false;

        timer FPS;
        FPS.start();
        int nFrames = 0;
        std::stringstream textFPS;
        IMG_wrapper imgFPS;
        bool showFPS = false;

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
                //p.evaluate(e);
                if(b1.evaluate(e) == b1.CLICK) help =  !help;
                if(b2.evaluate(e) == b2.CLICK) showFPS = !showFPS;
            }
            currentKeyStates = SDL_GetKeyboardState( NULL );
            p->evaluate(currentKeyStates);
            p->screen_position(sX,sY);

            screenRect = {sX,sY,wrapper.sWidth,wrapper.sHeight};

            SDL_RenderClear( wrapper.gRenderer );

            SDL_RenderSetViewport( wrapper.gRenderer, &screenWP );
            wrapper.render_image(img1, 0, 0, &screenRect);

            std::vector<std::shared_ptr<object>> collObjects;
            collObjects.push_back(c);
            collObjects.push_back(p);

            p->move(collObjects);
            p->plot(wrapper);
            c->plot(wrapper,&screenRect);

            b1.plot(wrapper);
            if(help) wrapper.render_image(textHelp,0,wrapper.sHeight/4+wrapper.TILESIZE/2 );
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
                wrapper.render_image(imgFPS,wrapper.sWidth-wrapper.TILESIZE,0);
                ++nFrames;
            }

            minimap.render_map(wrapper,img1,collObjects);
            SDL_SetRenderDrawColor( wrapper.gRenderer, 0x00, 0x00, 0x00, 0xFF );
            SDL_RenderPresent( wrapper.gRenderer );
        }
    }

    return 0;
}
