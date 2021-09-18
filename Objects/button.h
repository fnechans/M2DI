#include "base.h"
#include "IMG_wrapper.h"
#include "SDL_wrapper.h"


class button : public base
{
public:
    button( SDL_Rect pos, double ws=1, double hs=0.5);
    ~button(){}

	enum clipType {DEFAULT,HOVER,CLICK,COUNT};
	SDL_Rect clips[COUNT];

    SDL_Rect screenPos;
	uint state = DEFAULT;

    std::shared_ptr<IMG_wrapper> image;
    std::shared_ptr<IMG_wrapper> text;
	void plot( SDL_wrapper & wrapper );
	int evaluate( SDL_Event & event );
    bool set_image(std::string imagePath,std::string title="",SDL_Color color={255,255,150,255});
};
