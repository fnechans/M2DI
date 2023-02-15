#include "timer.h"

Timer::Timer()
{
    startTicks = 0;
    pauseTicks = 0;

    isStarted = false;
    isPaused = false;
}

void Timer::start()
{
    startTicks = SDL_GetTicks();
    pauseTicks = 0;

    isStarted = true;
    isPaused = false;
}

void Timer::stop()
{
    startTicks = 0;
    pauseTicks = 0;

    isStarted = false;
    isPaused = false;
}

void Timer::pause()
{
    if (isStarted && !isPaused)
    {
        isPaused = true;
        pauseTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void Timer::unpause()
{
    if (isStarted && isPaused)
    {
        isPaused = false;
        startTicks = SDL_GetTicks() - pauseTicks;
        pauseTicks = 0;
    }
}

Uint32 Timer::getTicks()
{
    Uint32 time = 0;

    if (isStarted)
    {
        if (isPaused)
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
