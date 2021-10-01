#ifndef SDL_H
#define SDL_H

#include "base.h"
#include "IMG_wrapper.h"

// TODO: move from class to functions and static variables
class SDL_wrapper : public base
{
public:
    SDL_wrapper(){}
    ~SDL_wrapper(){close();}

    //Starts up SDL and creates window
    bool init();
    //Frees media and shuts down SDL
    void close();
    // put img on surface
    void render_image(IMG_wrapper & img, int x = 0, int y = 0, SDL_Rect * clip = nullptr , double angle = 0 );
    void render_image( IMG_wrapper & img, SDL_Rect * renderQuad, SDL_Rect * clip = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE );
    // update the window ..
    void update_window();

    // clears everything
    void clear()
    {
        SDL_RenderClear( gRenderer );
    }
    void render()
    {
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderPresent( gRenderer );
    }

    bool isInit = false;
private:

    //The window we'll be rendering to
    SDL_Window * gWindow = nullptr;

};

#endif
