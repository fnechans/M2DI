#ifndef OBJECT_H
#define OBJECT_H

#include "gameplay.h"
#include "IMG_wrapper.h"
#include "SDL_wrapper.h"
#include "animation.h"

#include<vector>
#include<map>

class Object : public gameplay
{
public:
    Object(uint x=mWidth/2, uint y=mHeight/2);
    ~Object(){}

    enum direction {UP,DOWN,LEFT,RIGHT};
    static const char* const dirName[4];// = {"UP", "DOWN", "LEFT", "RIGHT"};

    std::shared_ptr<std::map<int,SDL_Rect>> clips;

    // global coor. vars
    SDL_Rect position;

    // local coor. vard
    int posSX;
    int posSY;
    // velocity based of external factors (knockback)
    int extVelX;
    int extVelY;
    // friction impacts extVel
    int friction;
    uint spriteType;
    SDL_Color mapColor;

    std::shared_ptr<IMG_wrapper> image;
    bool set_image(std::string imagePath);
    void plot( SDL_wrapper & wrapper, SDL_Rect * screen = nullptr );

    std::map<std::string,int> property;
    void set_health(uint value);
    void modify_health(int value);

    std::map<std::string,Animation> animations;
    void copy_animation(Object const & object );
    void plot_animation( SDL_wrapper & wrapper, SDL_Rect * screen = nullptr, bool pause = false);
    void set_animation(std::string animationName);

    bool dead = false;
private:
    std::string curAnimation = "";
};

std::vector<Object> import_map( std::string mapFile, int mapSizeX, int mapSizeY );

#endif // OBJECT_H
