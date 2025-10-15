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
    lua["TILESIZEINPUT"] = base::TILESIZEINPUT;
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
        auto cl = lua.new_usertype<Character>("Character", sol::constructors<Character()>());
        cl["image"] = &Character::image;
        cl["mapColor"] = &Character::mapColor;
        cl["add_animation"] = &Character::add_animation;
        cl["properties"] = &Character::properties;
        lua["create_checker"] = &create_checkers;
        lua["add_checker_int"] = &add_checker<int>;
        lua["add_checker_float"] = &add_checker<float>;
        lua["add_checker_string"] = &add_checker<std::string>;
        lua["add_checker_bool"] = &add_checker<bool>;
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
        cl["get_int"] = &Properties::get<int>;
        cl["get_float"] = &Properties::get<float>;
        cl["get_string"] = &Properties::get<std::string>;
        cl["get_bool"] = &Properties::get<bool>;
        cl["getp_int"] = &Properties::getp<int>;
        cl["getp_float"] = &Properties::getp<float>;
        cl["getp_string"] = &Properties::getp<std::string>;
        cl["getp_bool"] = &Properties::getp<bool>;
    }

    /// GRAPHICS
    {
        auto cl = lua.new_usertype<Animation>("Animation", sol::constructors<Animation()>());
        cl["image"] = &Animation::image;
    }
    {
        auto cl = lua.new_usertype<AnimationData>("AnimationData", sol::constructors<AnimationData()>());
        cl["renderMod"] = &AnimationData::renderMod;
        cl["set_frequency"] = &AnimationData::set_frequency;
        cl["rpt"] = &AnimationData::repeat;
        cl["clips"] = &AnimationData::clips;
        cl["get_animation"] = &AnimationData::get_animation;
    }
    {
        // tilemap
        sol::usertype<Map_wrapper> cl = lua.new_usertype<Map_wrapper>("Tilemap");
        cl["load_map"] = &Map_wrapper::load_map;
        cl["init"] = &Map_wrapper::init;
        cl["render_map"] = &Map_wrapper::render_map;
        cl["load_blocks"] = &Map_wrapper::load_blocks;
        cl["add_sprite_property"] = &Map_wrapper::add_sprite_property;
        cl["map_border_colision"] = &Map_wrapper::map_border_colision;
        cl["image"] = &Map_wrapper::image;
        cl["screen_position"] = &Map_wrapper::screen_position;
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
        auto cl = lua.new_usertype<Fl_Rect>("Fl_Rect", sol::constructors<Fl_Rect(), Fl_Rect(float, float, float, float)>());
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
        cl["add_button2"] = &Menu::add_button2;
        cl["get_state"] = &Menu::get_state;
    }
    {
        sol::usertype<Level> cl = lua.new_usertype<Level>("Level");
        cl["set_map"] = &Level::set_map;
        cl["get_map"] = &Level::get_map;
        cl["set_map_screen_position"] = &Level::set_map_screen_position;
        cl["add_character"] = &Level::add_character;
        cl["add_projectile"] = &Level::add_projectile;
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
        cl["l_quit"] = &Screen::l_quit;
        cl["l_pause"] = &Screen::l_pause;
        cl["l_nextScreen"] = &Screen::l_nextScreen;
        cl["l_property_update"] = &Screen::l_property_update;
        cl["l_screen_zoom"] = &Screen::l_screen_zoom;
        cl["schedule_plot"] = &Screen::schedule_plot;
        cl["l_plot_on_level"] = &Screen::l_plot_on_level;
        cl["l_plot_on_menu"] = &Screen::l_plot_on_menu;
        cl["l_plot_image"] = &Screen::l_plot_image;
    }
}
