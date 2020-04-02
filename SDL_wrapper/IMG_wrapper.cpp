#include "IMG_wrapper.h"

typedef IMG_wrapper IMGW;

void IMGW::clear()
{        
	SDL_DestroyTexture( image );
	image = nullptr;
	width = -1;
	height= -1;
	
	//Free global font
    TTF_CloseFont( gFont );
    gFont = nullptr;
}

void IMGW::set_color( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture
    SDL_SetTextureColorMod( image, red, green, blue );
}

bool IMGW::load_media( const char * path )
{
	//Load image at specified path
	SDL_Surface * loadedSurface = SDL_LoadBMP( path );
	if( loadedSurface == nullptr )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path, SDL_GetError() );
		return false;
	}
	// deletion is handled by surf_to_texture function
	return surf_to_texture(loadedSurface);
}

bool IMGW::load_text( std::string text, SDL_Color color, uint textSize, uint textWidth )
{
    //Open the font
	gFont = TTF_OpenFont( "data/lazy.ttf", textSize );
    if( gFont == nullptr )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        return false;
    }
	//Load image at specified path
    //SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, text.c_str(), color );
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped( gFont, text.c_str(), color, textWidth );

	if( textSurface == nullptr )
	{
		printf( "Unable to generate text %s! SDL Error: %s\n", text.c_str(), SDL_GetError() );
		return false;
	}
	// deletion is handled by surf_to_texture function
	return surf_to_texture(textSurface);
}

bool IMGW::surf_to_texture( SDL_Surface  * loadedSurface)
{
	// TMP assumes valid input... check?
	clear();

	SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

	    //Create texture from surface pixels
    image = SDL_CreateTextureFromSurface( this->gRenderer, loadedSurface );
	if( image == nullptr )
	{
		printf( "Unable to create texture! SDL Error: %s\n", SDL_GetError() );
		return false;
	}	
	width = loadedSurface->w;
	height = loadedSurface->h;

		//Get rid of old loaded surface
	SDL_FreeSurface( loadedSurface );

	return true;
}
