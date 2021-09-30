#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "base.h"

void reduce_to_zero( int & value, const int & reducer );

class gameplay : public base
{
public:
    gameplay();

    static SDL_Rect gameplayScreen;

};

#endif // GAMEPLAY_H
