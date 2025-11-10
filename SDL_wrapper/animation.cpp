#include "animation.h"
#include <exception>

Animation::Animation(const Animation &other)
{
    frame = 0;
    sprites = other.sprites;
    repeat = other.repeat;
    frequency = other.frequency;
}

SDL_Rect Animation::get_render_rect(SDL_Rect& screen, const SDL_Rect &position, double renderScale)
{
    if (sprites.size() == 0)
    {
        std::cout << "At least one sprite has to be added to animation!" << std::endl;
        // TODO default: sprites.push_back();
        return {0, 0, 0, 0};
    }
    SDL_Rect positionScreen = base::toScreen(screen, position, renderScale);
    SDL_Rect renderRect = sprites.at(frame / frequency)->shift;
    renderRect.x *= renderScale;
    renderRect.y *= renderScale;
    renderRect.w *= renderScale;
    renderRect.h *= renderScale;
    renderRect.x += positionScreen.x;
    renderRect.y += positionScreen.y;

    return renderRect;
}

void Animation::run()
{
    if (sprites.size() == 0)
    {
        std::cout << "At least one sprite has to be added to animation!" << std::endl;
        // TODO default: sprites.push_back();
        return;
    }
    if (running)
        ++frame;
    if (frequency == 0)
        throw std::runtime_error("Frequency cannot be 0!");
    if (frame >= frequency * sprites.size() - 1) // we are at the last frame or due to error beyond that
    {
        frame = 0;
        running = repeat; // the animation has ended
    }
}

void Animation::plot(Window& window, SDL_Rect* positionScreen, bool skipPlot)
{
    if (sprites.size() == 0)
    {
        std::cout << "At least one sprite has to be added to animation!" << std::endl;
        // TODO default: sprites.push_back();
        return;
    }
    if (skipPlot)
        return;

    sprites.at(frame / frequency)->plot(window, positionScreen);
}
