#ifndef ANIMATION_H
#define ANIMATION_H

#include "Tools/base.h"
#include "SDL_wrapper/sprite.h"

#include <vector>


class Animation
{
public:
    Animation(uint frequency = 10, bool repeat = true) : frequency(frequency), repeat(repeat) {}
    Animation(const Animation &other);
    void plot(Window& window, SDL_Rect* position, bool skipPlot = false);
    SDL_Rect get_render_rect(SDL_Rect& screen, const SDL_Rect &position, double renderScale);

    void run(); // increases frame and handles consequences

    // controls state of the animation
    void play() { running = true; }
    void pause() { running = false; }
    void stop()
    {
        running = false;
        frame = 0;
    }

    void add_sprite(Sprite* sprite) { sprites.push_back(sprite); }

    // TMP
    uint frequency = 60/8;
    double angle = 0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    bool repeat = true;
    bool running = false;

private:
    std::vector<Sprite*> sprites;
    uint frame{0};
};

class AnimationData
{
public:
    AnimationData()  {}
    // TMP: figure out how to best propagate ticks/frames per sec?
    uint frequency = 60/8;
    void set_frequency(double freq_seconds, uint fps) { frequency = uint(freq_seconds * fps); }
    bool repeat = true;
    std::vector<Sprite*> sprites;
    void add_sprite(Sprite* sprite) { sprites.push_back(sprite); }
    Animation get_animation()
    {
        Animation a;
        for (auto sprite : sprites) a.add_sprite(sprite);
        if (frequency == 0)
            frequency = 1;
        a.frequency = frequency;
        a.repeat = repeat;
        return a;
    }
};
#endif // ANIMATION_H
