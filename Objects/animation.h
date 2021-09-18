#ifndef ANIMATION_H
#define ANIMATION_H

#include "gameplay.h"
#include "IMG_wrapper.h"

#include <vector>

class animation : public gameplay
{
public:
    animation( int x = 0, int y = 0 ) : shiftX(x),shiftY(y){ frame = 0; }
    animation( const animation & other );
    void play(){ running = true; }
    void pause(){ running = false; }
    SDL_Rect get();
    void stop(){ running = false; frame = 0; }
    std::vector<SDL_Rect> clips;
    // shared because multiple animation will share it
    std::shared_ptr<IMG_wrapper> image; 
    bool set_image(std::string imagePath);

    int shiftX;
    int shiftY;
    uint frequency = 4;
    double angle = 0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    bool display = true;
    bool running = false;
private:
    uint frame;

};

#endif // ANIMATION_H
