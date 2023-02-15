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
        // TMP! How to best propagate?
        uint tmp_FRAMES_PER_SECOND = 60;

        animationData["DEFAULT_DOWN"].renderMod.x = -0.5;
        animationData["DEFAULT_DOWN"].renderMod.y = -1.25;
        animationData["DEFAULT_DOWN"].frequency = tmp_FRAMES_PER_SECOND / 2;
        animationData["DEFAULT_DOWN"].fclips = {
            {0, 0, 1, 2},
            {6, 0, 1, 2},
        };
        animationData["DEFAULT_RIGHT"].renderMod.x = -0.5;
        animationData["DEFAULT_RIGHT"].renderMod.y = -1.25;
        animationData["DEFAULT_RIGHT"].frequency = tmp_FRAMES_PER_SECOND / 2;
        animationData["DEFAULT_RIGHT"].fclips = {
            {0, 2, 1, 2},
            {6, 2, 1, 2},
        };
        animationData["DEFAULT_UP"].renderMod.x = -0.5;
        animationData["DEFAULT_UP"].renderMod.y = -1.25;
        animationData["DEFAULT_UP"].frequency = tmp_FRAMES_PER_SECOND / 2;
        animationData["DEFAULT_UP"].fclips = {
            {0, 4, 1, 2},
            {6, 4, 1, 2},
        };
        animationData["DEFAULT_LEFT"].renderMod.x = -0.5;
        animationData["DEFAULT_LEFT"].renderMod.y = -1.25;
        animationData["DEFAULT_LEFT"].frequency = tmp_FRAMES_PER_SECOND / 2;
        animationData["DEFAULT_LEFT"].fclips = {
            {0, 6, 1, 2},
            {6, 6, 1, 2},
        };
        animationData["WALK_DOWN"].renderMod.x = -0.5;
        animationData["WALK_DOWN"].renderMod.y = -1.25;
        animationData["WALK_DOWN"].frequency = tmp_FRAMES_PER_SECOND / 8;
        animationData["WALK_DOWN"].fclips = {
            {0, 0, 1, 2},
            {1, 0, 1, 2},
            {2, 0, 1, 2},
            {3, 0, 1, 2},
        };
        animationData["WALK_RIGHT"].renderMod.x = -0.5;
        animationData["WALK_RIGHT"].renderMod.y = -1.25;
        animationData["WALK_RIGHT"].frequency = tmp_FRAMES_PER_SECOND / 8;
        animationData["WALK_RIGHT"].fclips = {
            {0, 2, 1, 2},
            {1, 2, 1, 2},
            {2, 2, 1, 2},
            {3, 2, 1, 2},
        };
        animationData["WALK_UP"].renderMod.x = -0.5;
        animationData["WALK_UP"].renderMod.y = -1.25;
        animationData["WALK_UP"].frequency = tmp_FRAMES_PER_SECOND / 8;
        animationData["WALK_UP"].fclips = {
            {0, 4, 1, 2},
            {1, 4, 1, 2},
            {2, 4, 1, 2},
            {3, 4, 1, 2},
        };
        animationData["WALK_LEFT"].renderMod.x = -0.5;
        animationData["WALK_LEFT"].renderMod.y = -1.25;
        animationData["WALK_LEFT"].frequency = tmp_FRAMES_PER_SECOND / 8;
        animationData["WALK_LEFT"].fclips = {
            {0, 6, 1, 2},
            {1, 6, 1, 2},
            {2, 6, 1, 2},
            {3, 6, 1, 2},
        };
        animationData["ATTACK_DOWN"].renderMod.x = -1;
        animationData["ATTACK_DOWN"].renderMod.y = -1.25;
        animationData["ATTACK_DOWN"].frequency = tmp_FRAMES_PER_SECOND / 8;
        animationData["ATTACK_DOWN"].repeat = false;
        animationData["ATTACK_DOWN"].fclips = {
            {0, 8, 2, 2},
            {2, 8, 2, 2},
            {4, 8, 2, 2},
            {6, 8, 2, 2},
        };
        animationData["ATTACK_RIGHT"].renderMod.x = -1;
        animationData["ATTACK_RIGHT"].renderMod.y = -1.25;
        animationData["ATTACK_RIGHT"].frequency = tmp_FRAMES_PER_SECOND / 8;
        animationData["ATTACK_RIGHT"].repeat = false;
        animationData["ATTACK_RIGHT"].fclips = {
            {0, 12, 2, 2},
            {2, 12, 2, 2},
            {4, 12, 2, 2},
            {6, 12, 2, 2},
        };
        animationData["ATTACK_UP"].renderMod.x = -1;
        animationData["ATTACK_UP"].renderMod.y = -1.25;
        animationData["ATTACK_UP"].frequency = tmp_FRAMES_PER_SECOND / 8;
        animationData["ATTACK_UP"].repeat = false;
        animationData["ATTACK_UP"].fclips = {
            {0, 10, 2, 2},
            {2, 10, 2, 2},
            {4, 10, 2, 2},
            {6, 10, 2, 2},
        };
        animationData["ATTACK_LEFT"].renderMod.x = -1;
        animationData["ATTACK_LEFT"].renderMod.y = -1.25;
        animationData["ATTACK_LEFT"].frequency = tmp_FRAMES_PER_SECOND / 8;
        animationData["ATTACK_LEFT"].repeat = false;
        animationData["ATTACK_LEFT"].fclips = {
            {0, 14, 2, 2},
            {2, 14, 2, 2},
            {4, 14, 2, 2},
            {6, 14, 2, 2},
        };
        animationData["ATTACK"].renderMod.x = -1;
        animationData["ATTACK"].renderMod.y = -1;
        animationData["ATTACK"].renderMod.w = 3;
        animationData["ATTACK"].renderMod.h = 3;
        animationData["ATTACK"].repeat = false;
        animationData["ATTACK"].fclips = {
            {0, 0, 1, 1},
            {1, 0, 1, 1},
            {2, 0, 1, 1},
            {3, 0, 1, 1},
        };
        animationData["EXPLOSION"].renderMod.x = -1.5;
        animationData["EXPLOSION"].renderMod.y = -1.5;
        animationData["EXPLOSION"].renderMod.w = 3.*16/100;
        animationData["EXPLOSION"].renderMod.h = 3.*16/100;
        animationData["EXPLOSION"].repeat = false;
        animationData["EXPLOSION"].relative = false;
        animationData["EXPLOSION"].clips = {
            {0, 0, 100, 100},
            {0, 100, 100, 100},
            {0, 200, 100, 100},
            {0, 300, 100, 100},
            {0, 400, 100, 100},
            {0, 500, 100, 100},
        };
    }
}

#endif