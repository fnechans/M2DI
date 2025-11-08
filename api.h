#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "Screens/screen.h"
#include "Managers/image_manager.hpp"
#include "Managers/object_manager.hpp"

void init_enums(sol::state &lua)
{
    lua.new_enum("Position",
                 "CENTER", Menu::Position::CENTER,
                 "RIGHT", Menu::Position::RIGHT,
                 "LEFT", Menu::Position::LEFT,
                 "TOP", Menu::Position::TOP,
                 "BOTTOM", Menu::Position::BOTTOM,
                 "WHOLE", Menu::Position::WHOLE,
                 "RIGHT_FILL", Menu::Position::RIGHT_FILL,
                 "LEFT_FILL", Menu::Position::LEFT_FILL,
                 "TOP_FILL", Menu::Position::TOP_FILL,
                 "BOTTOM_FILL", Menu::Position::BOTTOM_FILL);
    uint fps = Screen::FRAMES_PER_SECOND;
    lua["FRAMES_PER_SECOND"] = fps;

    lua.new_enum("Direction",
                 "UP", Object::direction::UP,
                 "DOWN", Object::direction::DOWN,
                 "LEFT", Object::direction::LEFT,
                 "RIGHT", Object::direction::RIGHT);

    lua.new_enum("KEYS",
                 "a", SDLK_a,
                 "b", SDLK_b,
                 "c", SDLK_c,
                 "d", SDLK_d,
                 "e", SDLK_e,
                 "f", SDLK_f,
                 "g", SDLK_g,
                 "h", SDLK_h,
                 "i", SDLK_i,
                 "j", SDLK_j,
                 "k", SDLK_k,
                 "l", SDLK_l,
                 "m", SDLK_m,
                 "n", SDLK_n,
                 "o", SDLK_o,
                 "p", SDLK_p,
                 "q", SDLK_q,
                 "r", SDLK_r,
                 "s", SDLK_s,
                 "t", SDLK_t,
                 "u", SDLK_u,
                 "v", SDLK_v,
                 "w", SDLK_w,
                 "x", SDLK_x,
                 "y", SDLK_y,
                 "z", SDLK_z,
                 "escape", SDLK_ESCAPE,
                 "plus", SDLK_PLUS,
                 "minus", SDLK_MINUS);
}
std::vector<std::pair<std::string, PropertyType>> create_checkers() 
{ return {}; }

template <typename T>
void add_checker(std::vector<std::pair<std::string, PropertyType>> &checkers, std::string checker, T type) 
{ checkers.push_back({checker, type}); }

void create_classes(sol::state &lua)
{
    {
        sol::usertype<button> cl = lua.new_usertype<button>("button");
        cl["image"] = &button::image;
        cl["text"] = &button::text;
        cl["screenPosition"] = &button::screenPos;
    }
    {
        auto cl = lua.new_usertype<Block>("Block", sol::constructors<Block(), Block(uint, uint)>());
    }
    {
        auto cl = lua.new_usertype<Object>("Object", sol::constructors<Object()>());
        cl["image"] = &Object::image;
        cl["mapColor"] = &Object::mapColor;
        cl["add_animation"] = &Object::add_animation;
        cl["properties"] = &Object::properties;
        cl["hitbox"] = &Object::hitbox;
        cl["set_target"] = &Object::set_target;
        cl["doPlot"] = &Object::doPlot;
        cl["speedX"] = &Object::speedX;
        cl["speedY"] = &Object::speedY;
        lua["create_checker"] = &create_checkers;
        lua["add_checker"] = &add_checker<PropertyType>;
    }
    {
        auto cl = lua.new_usertype<KeyBinds>("KeyBinds", sol::constructors<KeyBinds()>());
        cl["add_keybind"] = &KeyBinds::add_keybind;
        cl["change_keybind"] = &KeyBinds::change_keybind;
        lua["l_player_move_up"] = &l_player_move_up;
        lua["l_player_move_down"] = &l_player_move_down;
        lua["l_player_move_left"] = &l_player_move_left;
        lua["l_player_move_right"] = &l_player_move_right;
    }
    {
        auto cl = lua.new_usertype<Properties>("Properties");
        cl["set"] = &Properties::set;
        cl["get_int"] = &Properties::getp<int>;
        cl["get_double"] = &Properties::getp<double>;
        cl["get_string"] = &Properties::getp<std::string>;
        cl["get_bool"] = &Properties::getp<bool>;
        cl["get"] = &Properties::operator[];
    }

    /// GRAPHICS
    {
        auto cl = lua.new_usertype<Animation>("Animation", sol::constructors<Animation()>());
        cl["image"] = &Animation::image;
    }
    {
        auto cl = lua.new_usertype<AnimationData>("AnimationData", sol::constructors<AnimationData()>());
        cl["set_frequency"] = &AnimationData::set_frequency;
        cl["rpt"] = &AnimationData::repeat;
        cl["clips"] = &AnimationData::clips;
        cl["get_animation"] = &AnimationData::get_animation;
    }
    {
        auto cl = lua.new_usertype<Sprite>("Sprite");
        cl["clip"] = &Sprite::clip;
        cl["image"] = &Sprite::image;
    }
    {
        auto cl = lua.new_usertype<Sprites>("Sprites");
        cl["add"] = &Sprites::add;
        cl["at"] = &Sprites::at;
    }
    {
        // tilemap
        sol::usertype<Map_wrapper> cl = lua.new_usertype<Map_wrapper>("Map_wrapper");
        cl["import_map"] = &Map_wrapper::import_map;
        cl["blank_map"] = &Map_wrapper::blank_map;
        cl["map_border_colision"] = &Map_wrapper::map_border_colision;
        cl["get_tile_pointers"] = &Map_wrapper::get_tile_pointers;
    }
    {
        auto cl = lua.new_usertype<AI<Block*>>("AI");
        cl["init"] = &AI<Block*>::init;
    }

    /// SDL
    {
        auto cl = lua.new_usertype<SDL_Rect>("SDL_Rect", sol::constructors<SDL_Rect(), SDL_Rect(int, int, int, int)>());
        cl["x"] = &SDL_Rect::x;
        cl["y"] = &SDL_Rect::y;
        cl["w"] = &SDL_Rect::w;
        cl["h"] = &SDL_Rect::h;
    }
    {
        auto cl = lua.new_usertype<Fl_Rect>("Fl_Rect", sol::constructors<Fl_Rect(), Fl_Rect(double, double, double, double)>());
        cl["x"] = &Fl_Rect::x;
        cl["y"] = &Fl_Rect::y;
        cl["w"] = &Fl_Rect::w;
        cl["h"] = &Fl_Rect::h;
    }
    {
        lua.new_usertype<SDL_Color>("SDL_Color", sol::constructors<SDL_Color(), SDL_Color(uint8_t, uint8_t, uint8_t, uint8_t)>());
    }

    /// MANAGERS
    {
        sol::usertype<ImageManager> cl = lua.new_usertype<ImageManager>("ImageManager");
        cl["add_image"] = &ImageManager::add_image;
        cl["get_image"] = &ImageManager::get_image;
        cl["delete_image"] = &ImageManager::delete_image;
        cl["add_text"] = &ImageManager::add_text;
        cl["get_text"] = &ImageManager::get_text;
        cl["delete_text"] = &ImageManager::delete_text;
    }

    /// SCREENS
    {
        sol::usertype<Menu> cl = lua.new_usertype<Menu>("Menu");
        cl["add_button_clip"] = &Menu::add_button_clip;
        cl["add_button_clips"] = &Menu::add_button_clips;
        cl["add_button_width_height"] = &Menu::add_button_width_height;
        cl["get_state"] = &Menu::get_state;
    }
    {
        sol::usertype<Level> cl = lua.new_usertype<Level>("Level");
        cl["set_map"] = &Level::set_map;
        cl["get_map"] = &Level::get_map;
        cl["get_blocks"] = &Level::get_blocks;
        cl["set_map_screen_position"] = &Level::set_map_screen_position;
        cl["add_character"] = &Level::add_character;
        cl["add_projectile"] = &Level::add_projectile;
        cl["set_ai"] = &Level::set_ai;
        cl["add_minimap"] = &Level::add_minimap;
    }
    {
        sol::usertype<Screen> cl = lua.new_usertype<Screen>("Screen");
        cl["add_menu2"] = &Screen::add_menu2;
        cl["add_level"] = &Screen::add_level;
        cl["quit"] = &Screen::quit;
        cl["nextScreen"] = &Screen::nextScreen;
        cl["properties"] = &Screen::properties;
        cl["keybinds"] = &Screen::keybinds;

        cl["schedule_button_update"] = &Screen::schedule_button_update;
        cl["schedule_screen_position_update"] = &Screen::schedule_screen_position_update;
        cl["schedule_screen_click_update"] = &Screen::schedule_screen_click_update;
        cl["schedule_tile_click_update"] = &Screen::schedule_tile_click_update;
        cl["l_quit"] = &Screen::l_quit;
        cl["l_pause"] = &Screen::l_pause;
        cl["l_nextScreen"] = &Screen::l_nextScreen;
        cl["l_property_update"] = &Screen::l_property_update;
        cl["l_screen_zoom"] = &Screen::l_screen_zoom;
        cl["l_update_tile_from_sprite"] = &Screen::l_update_tile_from_sprite;

        cl["schedule_plot"] = &Screen::schedule_plot;
        cl["l_plot_on_level"] = &Screen::l_plot_on_level;
        cl["l_plot_on_menu"] = &Screen::l_plot_on_menu;
        cl["l_plot_image"] = &Screen::l_plot_image;
    }
}
