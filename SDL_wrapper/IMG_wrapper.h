#ifndef IMG_H
#define IMG_H

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

//std
#include <string>
#include <stdio.h>

#include "base.h"

class IMG_wrapper : base
{
public:
    IMG_wrapper(){ gFont = nullptr; }
    ~IMG_wrapper(){clear();}

    //Loads media
    bool load_media( const char * path );
    bool load_text( std::string text, SDL_Color color, uint textSize = 32, uint textWidth = 32 );
    bool surf_to_texture( SDL_Surface  * loadedSurface );

    // Clear current image
    void clear();

    //Set color modulation
    void set_color( Uint8 red, Uint8 green, Uint8 blue );

    //The image we will load and show on the screen
    SDL_Texture * image = nullptr;
    int width = -1;
    int height= -1;
private:
    TTF_Font *gFont;
};
#endif
