#ifndef SPRITE_H
#define SPRITE_H

#include "IMG_wrapper.h"

#include <map>
struct Fl_Rect
{
    double x, y, w, h;
};

class Sprite
{
public:
    Sprite(IMG_wrapper* img, SDL_Rect clip, SDL_Color col = {0, 0, 0, 0}, Fl_Rect _shift = {0, 0, 0, 0})
     : image(img), color(col), clip(clip),
     shift{static_cast<int>(_shift.x * base::TILESIZEPHYSICS),
           static_cast<int>(_shift.y * base::TILESIZEPHYSICS),
           static_cast<int>(_shift.w * base::TILESIZEPHYSICS),
           static_cast<int>(_shift.h * base::TILESIZEPHYSICS)} {}
    Sprite(const Sprite& other){
        image = other.image;
        color = other.color;
        clip = other.clip;
        shift = other.shift;
    }
    IMG_wrapper* image{nullptr};
    SDL_Color color{0, 0, 0, 0};
    SDL_Rect clip{0, 0, 0, 0};
    SDL_Rect shift{0, 0, 0, 0};

    void plot(Window &window, SDL_Rect* renderRect)
    {
        if (!image) image = &IMG_wrapper::defaultImage;
        image->render_image(window, renderRect, &clip);
    }
};

class Sprites
{
public:
    Sprites() {}
    ~Sprites() {}
    void add(std::string name, IMG_wrapper* img, SDL_Rect clip, SDL_Color col, Fl_Rect shift = {0, 0, 0, 0})
    { sprites.emplace(name, std::make_unique<Sprite>(img, clip, col, shift)); }

    Sprite& at(std::string name) { return *sprites.at(name); }
    std::map<std::string, std::unique_ptr<Sprite>> sprites;
};

#endif
