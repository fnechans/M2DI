#ifndef BUTTON_H
#define BUTTON_H

#include "base.h"
#include "IMG_wrapper.h"

class button : public base
{
public:
    button(SDL_Rect pos, double ws=2, double hs=1);
    button(const button& b);
    ~button(){}

	enum clipType {DEFAULT,HOVER,CLICK,COUNT};
	SDL_Rect clips[COUNT];

    SDL_Rect screenPos;
	uint state = DEFAULT;

    std::shared_ptr<IMG_wrapper> image;
    std::shared_ptr<IMG_wrapper> text;
	void plot(Window & wrapper);
	int evaluate(SDL_Event & event, SDL_Rect viewPort);
    bool set_image(Window& window, std::string imagePath,std::string title="",SDL_Color color={255,255,150,255});
};

#endif //BUTTON_H