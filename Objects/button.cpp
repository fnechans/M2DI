#include "button.h"

button::button( SDL_Rect pos, double ws, double hs )
{
    screenPos = pos;

	for( int c = DEFAULT; c<COUNT; ++c )
	{
        clips[c] = { c*TILESIZEINPUT, 0, (int) (TILESIZEINPUT*ws), (int) (TILESIZEINPUT*hs) };
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
        if(x >= viewPort->x+screenPos.x 
           && x <= viewPort->x+screenPos.x+screenPos.w
           && y >= viewPort->y+screenPos.y
           && y <= viewPort->y+screenPos.y+screenPos.h
           && e.button.button == SDL_BUTTON_LEFT)
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
    wrapper.render_image(*image,&screenPos,&clips[state]);
    if(text) wrapper.render_image(*text,&screenPos);
}

bool button::set_image(std::string imagePath,std::string title,SDL_Color color)
{
    image =std::make_shared<IMG_wrapper>();
    if( !image->load_media(imagePath.c_str()) ) return false;
    if( title!="" )
    {
        text = std::make_shared< IMG_wrapper>();
        if( !text->load_text(title, color, screenPos.h) ) return false;
    }
    return true;
}
