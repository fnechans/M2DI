#include "button.h"

button::button(uint x, uint y, int ws, int hs )
{
	posX = x;
	posY = y;
    sizeX = TILESIZERENDER/ws;
    sizeY = TILESIZERENDER/hs;

	for( int c = DEFAULT; c<COUNT; ++c )
	{
        clips[c] = { c*TILESIZE, 0, TILESIZE, TILESIZE/2 };
	}	
}

int button::evaluate(SDL_Event & e)
{
    //If mouse event happened
    if( e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        if( x >= posX && x <= posX+sizeX && y >= posY && y <= posY+sizeY  )
        {
            //Set mouse over sprite
            switch( e.type )
            {
                case SDL_MOUSEMOTION:
                state = HOVER;
                break;
            
                case SDL_MOUSEBUTTONDOWN:
                state = CLICK;
                break;
                
                case SDL_MOUSEBUTTONUP:
                state = HOVER;
                break;
            }
        }
        else state = DEFAULT;
    }
    return state;
}

void button::plot( SDL_wrapper & wrapper )
{
    image->set_color(255,0,0);
    SDL_Rect r = {posX,posY,sizeX,sizeY};
    wrapper.render_image(*image,&r,&clips[state]);
    if(text) wrapper.render_image(*text,&r);
}

bool button::set_image(std::string imagePath,std::string title,SDL_Color color)
{
    image =std::make_shared<IMG_wrapper>();
    if( !image->load_media(imagePath.c_str()) ) return false;
    if( title!="" )
    {
        text = std::make_shared< IMG_wrapper>();
        if( !text->load_text(title, color, sizeY) ) return false;
    }
    return true;
}
