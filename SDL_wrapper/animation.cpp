#include "animation.h"
#include <exception>

Animation::Animation(const Animation &other)
{
    renderMod = other.renderMod;
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

bool Animation::set_image(Window &window, std::string imagePath)
{
    image = std::make_shared<IMG_wrapper>();
    return image->load_media(window, imagePath.c_str());
}

void Animation::add_clip_relative(std::vector<float> clip)
{
    if (clip.size() != 4)
        throw std::runtime_error("Add_clip_relative size not 4!");
    int xx = (int)(clip[0] * base::TILESIZEINPUT);
    int yy = (int)(clip[1] * base::TILESIZEINPUT);
    int ww = (int)(clip[2] * base::TILESIZEINPUT);
    int hh = (int)(clip[3] * base::TILESIZEINPUT);
    clips.push_back({xx, yy, ww, hh});
}

void Animation::run_and_plot(Window& window, SDL_Rect positionScreen, bool skipPlot)
{
    SDL_Rect spriteRect = get();
    if (skipPlot)
        return;
    SDL_Rect renderRect = {
        positionScreen.x + (int)(renderMod.x * base::TILESIZERENDER()),
        positionScreen.y + (int)(renderMod.y * base::TILESIZERENDER()),
        (int)(spriteRect.w * renderMod.w * base::scaleRenderInput()),
        (int)(spriteRect.h * renderMod.h * base::scaleRenderInput())};
    if(!image) throw std::runtime_error("No image in animation");
    // image->set_color(255,0,0);
    image->render_image(window, &renderRect, &spriteRect, angle, flip);
}