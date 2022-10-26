#include "base.h"

uint base::mWidth(0);
uint base::mHeight(0);
uint base::TILESIZERENDER(0);
uint base::TICKS_PER_SECOND(128);
uint base::FRAMES_PER_SECOND(60);
float base::DELTA_T(1. / base::TICKS_PER_SECOND);
bool base::VSYNC(false);
double base::scaleRender;
double base::scaleRenderInput;

base::base()
{
}

void base::set_tilerender(int TR)
{
    TILESIZERENDER = TR;
    scaleRender = ((double)TILESIZERENDER) / TILESIZEPHYSICS;
    scaleRenderInput = ((double)TILESIZERENDER) / TILESIZEINPUT;
}
