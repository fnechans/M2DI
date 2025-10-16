#include "window.h"

#include <SDL_ttf.h>
#include "SDL_gfx/SDL2_gfxPrimitives.h"

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
    if (!success)
        close();
    else printf("Window initialized\n");
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
void SDLW::clear()
{
    SDL_RenderClear(sdlRenderer);
}
// render everything
void SDLW::render()
{
    SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderPresent(sdlRenderer);
}
void SDLW::viewPort(SDL_Rect *viewPort)
{
    SDL_RenderSetViewport(sdlRenderer, viewPort);
}
void SDLW::drawColorRect(SDL_Rect *rect, const SDL_Color &color)
{
    SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(sdlRenderer, rect);
}
void SDLW::drawColorLine(const SDL_Rect& start, const SDL_Rect& end, const SDL_Color &color, uint width)
{
    SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
    //SDL_RenderDrawLine(sdlRenderer, start.x, start.y, end.x, end.y);
    thickLineRGBA(sdlRenderer, start.x, start.y, end.x, end.y, width, color.r, color.g, color.b, color.a);
}
void SDLW::drawColorTriangle(const SDL_Rect& p1, const SDL_Rect& p2, const SDL_Rect& p3, const SDL_Color &color, bool filled)
{
    SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
    //SDL_RenderDrawLine(sdlRenderer, start.x, start.y, end.x, end.y);
    if(filled) filledTrigonRGBA(sdlRenderer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color.r, color.g, color.b, color.a);
    else trigonRGBA(sdlRenderer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color.r, color.g, color.b, color.a);
}