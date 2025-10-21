#include "animation.h"
#include <exception>

Animation::Animation(const Animation &other)
{
    image = other.image;
    frame = 0;
    clips = other.clips;
    repeat = other.repeat;
    frequency = other.frequency;
}

SDL_Rect Animation::get()
{
    if (clips.size() == 0)
        throw std::runtime_error("At least one clip has to be added to animation!");
    if (running)
        ++frame;
    if (frequency == 0)
        throw std::runtime_error("Frequency cannot be 0!");
    uint pos = frame / frequency;
    if (frame >= frequency * clips.size() - 1) // we are at the last frame or due to error beyond that
    {
        frame = 0;
        running = repeat; // the animation has ended
    }
    return clips.at(pos);
}

void Animation::run_and_plot(Window& window, SDL_Rect positionScreen, bool skipPlot)
{
    SDL_Rect spriteRect = get();
    if (skipPlot)
        return;
    if(!image) throw std::runtime_error("No image in animation");
    image->render_image(window, &positionScreen, &spriteRect, angle, flip);
}
