#ifndef SDL_H
#define SDL_H

#include "base.h"

// TODO: move from class to functions and static variables
class Window
{
public:
    Window(){}
    ~Window(){close();}

    //Starts up SDL and creates window
    bool init();
    //Frees media and shuts down SDL
    void close();
    // update the window ..
    void update_window();

    void clear()
    {
        SDL_RenderClear( sdlRenderer );
    }
    // render everything
    void render()
    {
        SDL_SetRenderDrawColor( sdlRenderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderPresent( sdlRenderer );
    }
    void viewPort(SDL_Rect* viewPort)
    {
        SDL_RenderSetViewport(sdlRenderer, viewPort);
    }
    void drawColorRect(SDL_Rect* rect, SDL_Color& color)
    {
        SDL_SetRenderDrawColor( sdlRenderer, color.r, color.g, color.b, color.a );
        SDL_RenderFillRect( sdlRenderer, rect );
    }
    bool isInit = false;

	//Screen dimension constants
    int sWidth;
    int sHeight;
    SDL_Renderer * sdlRenderer = nullptr;
private:

    //The window we'll be rendering to
    SDL_Window * sdlWindow = nullptr;

};

#endif