#include "SDL_wrapper.h"

typedef SDL_wrapper SDLW;

bool SDLW::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sWidth, sHeight, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
		            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
            	printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            	success = false;
            }
            else
            {
                //Initialize renderer color
            	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            }

                 //Initialize SDL_ttf
            if( TTF_Init() == -1 )
            {
            	printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
            	success = false;
            }
        }
    }

    return success;
}

void SDLW::close()
{
	//Destroy window
    SDL_DestroyRenderer( gRenderer );
    gRenderer = nullptr;
	SDL_DestroyWindow( gWindow );
	gWindow = nullptr;

    //Quit SDL subsystems
    TTF_Quit();
//    IMG_Quit();
    SDL_Quit();
}

void SDLW::render_image( IMG_wrapper & img, int x, int y, SDL_Rect * clip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, img.width, img.height };

    //Set clip rendering dimensions
    if( clip != nullptr )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    render_image(img,&renderQuad,clip);
}

void SDLW::render_image( IMG_wrapper & img, SDL_Rect * renderQuad, SDL_Rect * clip )
{
    //Render texture to screen
	SDL_RenderCopy( gRenderer, img.image, clip, renderQuad );
}

void SDLW::update_window()
{
	SDL_UpdateWindowSurface(gWindow);
}
