#include "timer.h"

timer::timer()
{
    startTicks = 0;
    pauseTicks = 0;

    isStarted = false;
    isPaused  = false;
}

void timer::start()
{
    startTicks = SDL_GetTicks();
    pauseTicks = 0;

    isStarted = true;
    isPaused  = false;
}

void timer::stop()
{
    startTicks = 0;
    pauseTicks = 0;

    isStarted = false;
    isPaused  = false;
}

void timer::pause()
{
    if( isStarted && !isPaused )
    {
        isPaused = true;
        pauseTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void timer::unpause()
{
    if( isStarted && isPaused )
    {
        isPaused = false;
        startTicks = SDL_GetTicks() - pauseTicks;
        pauseTicks = 0;
    }
}

Uint32 timer::getTicks()
{
    Uint32 time = 0;

    if( isStarted )
    {
        if( isPaused )
        {
            time = pauseTicks;
        }
        else
        {
            time = SDL_GetTicks() - startTicks;
        }
    }

    return time;
}
