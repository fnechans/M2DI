
#include "base.h"
#include "IMG_wrapper.h"
#include "SDL_wrapper.h"


class button : public base
{
public:
    button(uint x=sWidth/2,uint y=sHeight/2,int ws=2, int hs=4);
    ~button(){}

	enum clipType {DEFAULT,HOVER,CLICK,COUNT};
	SDL_Rect clips[COUNT];

	int posX;
	int posY;
	int sizeX;
	int sizeY;	
	uint state = DEFAULT;

    std::shared_ptr<IMG_wrapper> image;
    std::shared_ptr<IMG_wrapper> text;
	void plot( SDL_wrapper & wrapper );
	int evaluate( SDL_Event & event );
    bool set_image(std::string imagePath,std::string title="",SDL_Color color={255,255,150,255});
};
