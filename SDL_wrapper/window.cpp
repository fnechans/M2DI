#include "window.h"

#include <SDL_ttf.h>

typedef Window SDLW;

bool SDLW::init()
{
    //Initialization flag
    bool success = true;
    sWidth = 64 * 8 * 2;
    sHeight = 64 * 6 * 2;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Create window
        sdlWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sWidth, sHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (sdlWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create renderer for window
            if(VSYNC)
                sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            else
                sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
            if (sdlRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }

            //Initialize SDL_ttf
            if (TTF_Init() == -1)
            {
                printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                success = false;
            }
        }
    }
    isInit = success;

    return success;
}

void SDLW::close()
{
    //Destroy window
    SDL_DestroyRenderer(sdlRenderer);
    sdlRenderer = nullptr;
    SDL_DestroyWindow(sdlWindow);
    sdlWindow = nullptr;

    //Quit SDL subsystems
    TTF_Quit();
    //    IMG_Quit();
    SDL_Quit();
}

void SDLW::update_window()
{
    SDL_UpdateWindowSurface(sdlWindow);
}
