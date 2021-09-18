#include "animation.h"
#include <exception>

animation::animation(const animation &other )
{
    shiftX = other.shiftX;
    shiftY = other.shiftY;
    image  = other.image;
    frame = 0;
    clips = other.clips;
    display = other.display;
    frequency = other.frequency;
}

SDL_Rect animation::get()
{
    if(clips.size()==0) throw std::runtime_error("At least one clip has to be added to animation!");
    if(running) ++frame;
    uint pos = frame/frequency;
    if( frame >= frequency*clips.size()-1 )  // we are at the last frame or due to error beyond that
    {
        frame = 0;
        running = false; // the animation has ended
    }
    return clips.at(pos);
}

bool animation::set_image(std::string imagePath)
{
    image = std::make_shared<IMG_wrapper>();
    return image->load_media(imagePath.c_str());
}
