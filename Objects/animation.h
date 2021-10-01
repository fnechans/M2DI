#ifndef ANIMATION_H
#define ANIMATION_H

#include "base.h"
#include "IMG_wrapper.h"

#include <vector>

class Animation : public base
{
public:
    Animation( int x = 0, int y = 0 ) : shiftX(x),shiftY(y){ frame = 0; }
    Animation( const Animation & other );
    void play(){ running = true; }
    void pause(){ running = false; }
    SDL_Rect get();
    void stop(){ running = false; frame = 0; }
    // shared because multiple animation will share it
    std::shared_ptr<IMG_wrapper> image; 
    bool set_image(Window&, std::string imagePath);
    void add_clip(SDL_Rect clip){clips.push_back(clip);}
    void add_clip_relative(std::vector<float> clip);

    int shiftX;
    int shiftY;
    uint frequency = base::TILESIZEPHYSICS/16;
    float angle = 0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    bool repeat = true;
    bool running = false;
    std::vector<SDL_Rect> clips;
private:
    uint frame;

};

class animation_data : base
{
public:
    animation_data() : shiftX(0), shiftY(0){}
    float shiftX;
    float shiftY;
    uint frequency = TILESIZEPHYSICS/16;
    std::vector<std::vector<float>> clips;
    bool repeat = true;
    Animation get_animation()
    {
        Animation a((int)(shiftX*TILESIZEPHYSICS), (int)(shiftY*TILESIZEPHYSICS));
        for(auto clip :clips)
            a.add_clip_relative(clip);
        if(frequency==0) frequency = 1;
        a.frequency = frequency;
        a.repeat = repeat;
        return a;
    }
};
#endif // ANIMATION_H
