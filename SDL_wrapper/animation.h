#ifndef ANIMATION_H
#define ANIMATION_H

#include "Tools/base.h"
#include "SDL_wrapper/IMG_wrapper.h"

#include <vector>

struct Fl_Rect
{
    double x, y, w, h;
};

class Animation
{
public:
    Animation() { frame = 0; }
    Animation(IMG_wrapper* _image) : image(_image) { frame = 0; }
    Animation(const Animation &other);
    void play() { running = true; }
    void pause() { running = false; }
    SDL_Rect get();
    void run_and_plot(Window& window, SDL_Rect position, bool skipPlot = false);
    void stop()
    {
        running = false;
        frame = 0;
    }

    IMG_wrapper* image;
    void add_clip(SDL_Rect clip) { clips.push_back(clip); }

    // TMP
    uint frequency = 60/8;
    double angle = 0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    bool repeat = true;
    bool running = false;
    std::vector<SDL_Rect> clips;

private:
    uint frame;
};

class AnimationData
{
public:
    AnimationData()  {}
    // TMP: figure out how to best propagate ticks/frames per sec?
    uint frequency = 60/8;
    void set_frequency(double freq_sec, uint fps) { frequency = uint(freq_sec * fps); }
    std::vector<std::vector<double>> fclips;
    std::vector<SDL_Rect> clips;
    bool repeat = true;
    Animation get_animation()
    {
        Animation a;
        for (auto clip : clips) a.add_clip(clip);
        if (frequency == 0)
            frequency = 1;
        a.frequency = frequency;
        a.repeat = repeat;
        return a;
    }
};
#endif // ANIMATION_H
