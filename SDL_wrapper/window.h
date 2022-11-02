#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include "base.h"

// TODO: move from class to functions and static variables
class Window
{
public:
    Window() {}
    ~Window() { close(); }

    //Starts up SDL and creates window
    bool init(bool vsync = false);
    //Frees media and shuts down SDL
    void close();
    // update the window ..
    void update_window();

    void clear()
    {
        SDL_RenderClear(sdlRenderer);
    }
    // render everything
    void render()
    {
        SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderPresent(sdlRenderer);
    }
    void viewPort(SDL_Rect *viewPort)
    {
        SDL_RenderSetViewport(sdlRenderer, viewPort);
    }
    void drawColorRect(SDL_Rect *rect, const SDL_Color &color)
    {
        SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(sdlRenderer, rect);
    }
    void drawColorLine(const SDL_Rect& start, const SDL_Rect& end, const SDL_Color &color, uint width=1)
    {
        SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
        //SDL_RenderDrawLine(sdlRenderer, start.x, start.y, end.x, end.y);
        thickLineRGBA(sdlRenderer, start.x, start.y, end.x, end.y, width, color.r, color.g, color.b, color.a);
    }
    void drawColorTriangle(const SDL_Rect& p1, const SDL_Rect& p2, const SDL_Rect& p3, const SDL_Color &color, bool filled = true)
    {
        SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
        //SDL_RenderDrawLine(sdlRenderer, start.x, start.y, end.x, end.y);
        if(filled) filledTrigonRGBA(sdlRenderer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color.r, color.g, color.b, color.a);
        else trigonRGBA(sdlRenderer, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color.r, color.g, color.b, color.a);
    }
    bool isInit = false;

    //Screen dimension constants
    int sWidth;
    int sHeight;
    SDL_Renderer *sdlRenderer = nullptr;

private:
    //The window we'll be rendering to
    SDL_Window *sdlWindow = nullptr;
};

#endif