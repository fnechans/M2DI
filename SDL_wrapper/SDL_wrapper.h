#ifndef SDL_H
#define SDL_H

#include "base.h"
#include "IMG_wrapper.h"

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
    void render_image( IMG_wrapper & img, int x = 0, int y = 0, SDL_Rect * clip = nullptr );
    void render_image( IMG_wrapper & img, SDL_Rect * renderQuad, SDL_Rect * clip = nullptr );
    // update the window ..
    void update_window();

    SDL_Renderer * gRenderer;
private:

    //The window we'll be rendering to
    SDL_Window * gWindow = nullptr;

};

#endif
