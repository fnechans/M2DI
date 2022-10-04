#ifndef IMG_H
#define IMG_H

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//std
#include <string>
#include <stdio.h>

#include "window.h"

class IMG_wrapper
{
public:
    IMG_wrapper() { gFont = nullptr; }
    ~IMG_wrapper() { clear(); }

    //Loads media
    bool load_media(Window &, const char *path);
    bool load_text(Window &, std::string text, SDL_Color color, uint textSize = 32, uint textWidth = 32);
    // put img on surface
    void render_image(Window &, int x = 0, int y = 0, SDL_Rect *clip = nullptr, double angle = 0);
    void render_image(Window &, SDL_Rect *renderQuad, SDL_Rect *clip = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE);

    // Clear current image
    void clear();

    //Set color modulation
    void set_color(Uint8 red, Uint8 green, Uint8 blue);

    //The image we will load and show on the screen
    SDL_Texture *image = nullptr;
    int width = -1;
    int height = -1;

private:
    bool surf_to_texture(SDL_Renderer *, SDL_Surface *loadedSurface);
    TTF_Font *gFont;
};
#endif
