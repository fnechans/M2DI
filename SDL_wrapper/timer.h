#ifndef TIMER_H
#define TIMER_H

#include "base.h"

class Timer : public base
{
public:
    Timer();

    void start();
    void stop();
    void pause();
    void unpause();
    void restart()
    {
        stop();
        start();
    }
    Uint32 getTicks();

    bool isStarted;
    bool isPaused;

    Uint32 startTicks;
    Uint32 pauseTicks;
};

#endif // TIMER_H