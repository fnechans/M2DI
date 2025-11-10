#ifndef SDLWINDOW_H
#define SDLWINDOW_H

#include "Tools/base.h"

// TODO: move from class to functions and static variables
class Window
{
public:
    Window(bool vsync = false) : VSYNC(vsync) {}
    ~Window() { close(); }

    //Starts up SDL and creates window
    bool init();
    //Frees media and shuts down SDL
    void close();
    // update the window ..
    void update_window();

    void clear();
    // render everything
    void render();
    void viewPort(SDL_Rect *viewPort);
    void drawColorRect(SDL_Rect *rect, const SDL_Color &color);
    void drawColorLine(const SDL_Rect& start, const SDL_Rect& end, const SDL_Color &color, uint width=1);
    void drawColorTriangle(const SDL_Rect& p1, const SDL_Rect& p2, const SDL_Rect& p3, const SDL_Color &color, bool filled = true);
    bool isInit = false;

    //Screen dimension constants
    int sWidth;
    int sHeight;
    SDL_Renderer *sdlRenderer = nullptr;

    bool vsync() { return VSYNC; }
    void toggleFullscreen();
private:
    //The window we'll be rendering to
    SDL_Window *sdlWindow = nullptr;
    bool VSYNC;
    bool fullscreen = false;
};

#endif