#include "gameplay.h"

SDL_Rect gameplay::gameplayScreen={0,0,0,0};

gameplay::gameplay()
{
}

void reduce_to_zero( int & value, const int & reducer )
{
    if( value>0 )
    {
        value -= reducer;
        if( value<0 ) value = 0;
    }
    if( value<0 )
    {
        value += reducer;
        if( value>0 ) value = 0;
    }
}
