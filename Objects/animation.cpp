#include "animation.h"
#include <exception>

Animation::Animation(const Animation &other )
{
    shiftX = other.shiftX;
    shiftY = other.shiftY;
    image  = other.image;
    frame = 0;
    clips = other.clips;
    repeat = other.repeat;
    frequency = other.frequency;
}

SDL_Rect Animation::get()
{
    if(clips.size()==0) throw std::runtime_error("At least one clip has to be added to animation!");
    if(running) ++frame;
    if(frequency == 0) throw std::runtime_error("Frequency cannot be 0!");
    uint pos = frame/frequency;
    if( frame >= frequency*clips.size()-1 )  // we are at the last frame or due to error beyond that
    {
        frame = 0;
        running = repeat; // the animation has ended
    }
    return clips.at(pos);
}

bool Animation::set_image(std::string imagePath)
{
    image = std::make_shared<IMG_wrapper>();
    return image->load_media(imagePath.c_str());
}

void Animation::add_clip_relative(std::vector<float> clip)
{
    if(clip.size()!=4) throw std::runtime_error("Add_clip_relative size not 4!");
    int xx = (int) (clip[0]*TILESIZEINPUT);
    int yy = (int) (clip[1]*TILESIZEINPUT);
    int ww = (int) (clip[2]*TILESIZEINPUT);
    int hh = (int) (clip[3]*TILESIZEINPUT);
    clips.push_back({xx, yy, ww, hh});
}