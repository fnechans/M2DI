#ifndef PRESET_H
#define PRESET_H

#include "animation.h"
// temporary class to hold animation
// will be replaced by loader

namespace preset
{
    std::map<std::string, animation_data> animationData;
    void get_animation_data()
    {
        animationData["DEFAULT_DOWN"].shiftY = -0.75;
        animationData["DEFAULT_DOWN"].frequency = base::TILESIZE/4;
        animationData["DEFAULT_DOWN"].clips = {
            {0, 0, 1, 2},
            {6, 0, 1, 2},
        };
        animationData["DEFAULT_RIGHT"].shiftY = -0.75;
        animationData["DEFAULT_RIGHT"].frequency = base::TILESIZE/4;
        animationData["DEFAULT_RIGHT"].clips = {
            {0, 2, 1, 2},
            {6, 2, 1, 2},
        };
        animationData["DEFAULT_UP"].shiftY = -0.75;
        animationData["DEFAULT_UP"].frequency = base::TILESIZE/4;
        animationData["DEFAULT_UP"].clips = {
            {0, 4, 1, 2},
            {6, 4, 1, 2},
        };
        animationData["DEFAULT_LEFT"].shiftY = -0.75;
        animationData["DEFAULT_LEFT"].frequency = base::TILESIZE/4;
        animationData["DEFAULT_LEFT"].clips = {
            {0, 6, 1, 2},
            {6, 6, 1, 2},
        };
        animationData["WALK_DOWN"].shiftY = -0.75;
        animationData["WALK_DOWN"].clips = {
            {0, 0, 1, 2},
            {1, 0, 1, 2},
            {2, 0, 1, 2},
            {3, 0, 1, 2},
        };
        animationData["WALK_RIGHT"].shiftY = -0.75;
        animationData["WALK_RIGHT"].clips = {
            {0, 2, 1, 2},
            {1, 2, 1, 2},
            {2, 2, 1, 2},
            {3, 2, 1, 2},
        };
        animationData["WALK_UP"].shiftY = -0.75;
        animationData["WALK_UP"].clips = {
            {0, 4, 1, 2},
            {1, 4, 1, 2},
            {2, 4, 1, 2},
            {3, 4, 1, 2},
        };
        animationData["WALK_LEFT"].shiftY = -0.75;
        animationData["WALK_LEFT"].clips = {
            {0, 6, 1, 2},
            {1, 6, 1, 2},
            {2, 6, 1, 2},
            {3, 6, 1, 2},
        };
        animationData["ATTACK_DOWN"].shiftX = -0.5;
        animationData["ATTACK_DOWN"].shiftY = -0.75;
        animationData["ATTACK_DOWN"].frequency = base::TILESIZE/32;
        animationData["ATTACK_DOWN"].repeat = false;
        animationData["ATTACK_DOWN"].clips = {
            {0, 8, 2, 2},
            {2, 8, 2, 2},
            {4, 8, 2, 2},
            {6, 8, 2, 2},
        };
        animationData["ATTACK_RIGHT"].shiftX = -0.5;
        animationData["ATTACK_RIGHT"].shiftY = -0.75;
        animationData["ATTACK_RIGHT"].frequency = base::TILESIZE/32;
        animationData["ATTACK_RIGHT"].repeat = false;
        animationData["ATTACK_RIGHT"].clips = {
            {0, 12, 2, 2},
            {2, 12, 2, 2},
            {4, 12, 2, 2},
            {6, 12, 2, 2},
        };
        animationData["ATTACK_UP"].shiftX = -0.5;
        animationData["ATTACK_UP"].shiftY = -0.75;
        animationData["ATTACK_UP"].frequency = base::TILESIZE/32;
        animationData["ATTACK_UP"].repeat = false;
        animationData["ATTACK_UP"].clips = {
            {0, 10, 2, 2},
            {2, 10, 2, 2},
            {4, 10, 2, 2},
            {6, 10, 2, 2},
        };
        animationData["ATTACK_LEF"].shiftX = -0.5;
        animationData["ATTACK_LEFT"].shiftY = -0.75;
        animationData["ATTACK_LEFT"].frequency = base::TILESIZE/32;
        animationData["ATTACK_LEFT"].repeat = false;
        animationData["ATTACK_LEFT"].clips = {
            {0, 14, 2, 2},
            {2, 14, 2, 2},
            {4, 14, 2, 2},
            {6, 14, 2, 2},
        };
        animationData["ATTACK"].shiftX = 0;
        animationData["ATTACK"].shiftY = 0.25;
        animationData["ATTACK"].repeat = false;
        animationData["ATTACK"].clips = {
            {0, 0, 1, 1},
            {1, 0, 1, 1},
            {2, 0, 1, 1},
            {3, 0, 1, 1},
        };
    }
}
                /*
                if(player.dir==player.LEFT)
                {
                    player.animations["ATTACK"]->angle = 0;
                    player.animations["ATTACK"]->flip  = SDL_FLIP_NONE;
                    player.animations["ATTACK"]->shiftX= -player.TILESIZE;
                    player.animations["ATTACK"]->shiftY= -player.TILESIZE;
                }
                else if(player.dir==player.RIGHT)
                {
                    player.animations["ATTACK"]->angle = 0;
                    player.animations["ATTACK"]->flip  = SDL_FLIP_HORIZONTAL;
                    player.animations["ATTACK"]->shiftX= +player.TILESIZE;
                    player.animations["ATTACK"]->shiftY= -player.TILESIZE;
                }
                else if(player.dir==player.UP)
                {
                    player.animations["ATTACK"]->angle = 90;
                    player.animations["ATTACK"]->flip  = SDL_FLIP_NONE;
                    player.animations["ATTACK"]->shiftX= 0;
                    player.animations["ATTACK"]->shiftY= -2*player.TILESIZE;
                }
                else if(player.dir==player.DOWN)
                {
                    player.animations["ATTACK"]->angle = 270;
                    player.animations["ATTACK"]->flip  = SDL_FLIP_NONE;
                    player.animations["ATTACK"]->shiftX= 0;
                    player.animations["ATTACK"]->shiftY= 0;
                }
                player.animations["ATTACK"].play();
                */
#endif