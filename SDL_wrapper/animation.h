#ifndef ANIMATION_H
#define ANIMATION_H

#include "base.h"
#include "IMG_wrapper.h"

#include <vector>

struct Fl_Rect
{
    float x, y, w, h;
};

class Animation
{
public:
    Animation(Fl_Rect _renderMod = {0, 0, 1, 1}) : renderMod(_renderMod) { frame = 0; }
    Animation(std::shared_ptr<IMG_wrapper> _image, Fl_Rect _renderMod = {0, 0, 1, 1}) : renderMod(_renderMod), image(_image) { frame = 0; }
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

    Fl_Rect renderMod; // x,y shifts, w,h scales of picture when rendering
    // shared because multiple animation will share it
    std::shared_ptr<IMG_wrapper> image;
    bool set_image(Window &, std::string imagePath);
    void add_clip(SDL_Rect clip) { clips.push_back(clip); }
    void add_clip_relative(std::vector<float> clip);

    // TMP
    uint frequency = 60/8;
    float angle = 0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    bool repeat = true;
    bool running = false;
    std::vector<SDL_Rect> clips;

private:
    uint frame;
};

class animation_data
{
public:
    animation_data()  {}
    Fl_Rect renderMod{0, 0, 1, 1};
    // TMP: figure out how to best propagate ticks/frames per sec?
    uint frequency = 60/8;
    std::vector<std::vector<float>> fclips;
    std::vector<SDL_Rect> clips;
    bool repeat = true;
    bool relative = true;
    Animation get_animation()
    {
        Animation a(renderMod);
        if(relative)
        {
            for (auto clip : fclips) a.add_clip_relative(clip);
        }
        else
        {
            for (auto clip : clips) a.add_clip(clip);
        }
        if (frequency == 0)
            frequency = 1;
        a.frequency = frequency;
        a.repeat = repeat;
        return a;
    }
};
#endif // ANIMATION_H
