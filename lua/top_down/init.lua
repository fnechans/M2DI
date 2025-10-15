
-- Map
print(" > initializing map")
function rect(x, y, w, h)
    return SDL_Rect.new(x, y, w, h)
end

function col(r, g, b, a)
    return SDL_Color.new(r, g, b, a)
end

level = screen:add_level()
level:set_map(SDL_Rect.new(0, 0, 0, 0))
map = level:get_map()
map:init(40, 36)

function add_sprite(x, y, r, g, b, a, obscure)
    print("adding sprite at " .. x .. ", " .. y)
    c = SDL_Color.new(r, g, b, a)
    map:add_sprite_property(x, y, c, obscure)
end

add_sprite(1, 4, 100, 100, 0, 255, true)
add_sprite(0, 0, 0, 150, 0, 255, true) -- grass
add_sprite(12, 13, 100, 100, 100, 255, true)
add_sprite(4, 5, 150, 100, 50, 255, true)
add_sprite(0, 1, 0, 0, 150, 255, false) -- water

map.image = imageManager:get_image('map')
map:load_map("data/start.map", 16, 16)
map:load_blocks("data/startBlock2.map", 16, 16)


-- Animations
print(" > initializing animations")
local animationHolder = {}
function rect_tile(x, y, w, h)
    return SDL_Rect.new(x*TILESIZEINPUT, y*TILESIZEINPUT, w*TILESIZEINPUT, h*TILESIZEINPUT)
end
function create_animation(name, x, y, frequency, clips ,rpt)
    local animation = AnimationData.new()
    animation.renderMod.x = x
    animation.renderMod.y = y
    animation:set_frequency(frequency, FRAMES_PER_SECOND)
    animation.clips = clips
    animation.rpt = rpt
    animationHolder[name] = animation
    return animation
end


create_animation("DEFAULT_DOWN", -0.5, -1.25, 1, {rect_tile(0, 0, 1, 2), rect_tile(6, 0, 1, 2)}, true)
create_animation("DEFAULT_RIGHT", -0.5, -1.25, 1, {rect_tile(0, 2, 1, 2), rect_tile(6, 2, 1, 2)}, true)
create_animation("DEFAULT_UP", -0.5, -1.25, 1, {rect_tile(0, 4, 1, 2), rect_tile(6, 4, 1, 2)}, true)
create_animation("DEFAULT_LEFT", -0.5, -1.25, 1, {rect_tile(0, 6, 1, 2), rect_tile(6, 6, 1, 2)}, true)

create_animation("WALK_DOWN", -0.5, -1.25, 0.15, {
    rect_tile(0, 0, 1, 2), rect_tile(1, 0 ,1 ,2), rect_tile(2, 0, 1, 2), rect_tile(3, 0, 1, 2)}, true)
create_animation("WALK_RIGHT", -0.5, -1.25, 0.15, {
    rect_tile(0, 2, 1, 2), rect_tile(1, 2 ,1 ,2), rect_tile(2, 2, 1, 2), rect_tile(3, 2, 1, 2)}, true)
create_animation("WALK_UP", -0.5, -1.25, 0.15, {
    rect_tile(0, 4, 1, 2), rect_tile(1, 4 ,1 ,2), rect_tile(2, 4, 1, 2), rect_tile(3, 4, 1, 2)}, true)
create_animation("WALK_LEFT", -0.5, -1.25, 0.15, {
    rect_tile(0, 6, 1, 2), rect_tile(1, 6 ,1 ,2), rect_tile(2, 6, 1, 2), rect_tile(3, 6, 1, 2)}, true)


-- Add chars
print(" > adding characters")
player = level:add_character("player", 9, 9);
player.image = imageManager:get_image('player')
player.mapColor = SDL_Color.new(0, 250, 0, 255)

function add_animation(_player, _animdata, moved, direction)
    _anim = _animdata:get_animation()
    _anim.image = _player.image
    checkers = create_checker()
    add_checker_bool(checkers, "moved", moved)
    add_checker_int(checkers, "direction", direction)
    _player:add_animation(_anim, checkers)
end

add_animation(player, animationHolder["DEFAULT_DOWN"], false, Direction.DOWN)
add_animation(player, animationHolder["DEFAULT_RIGHT"], false, Direction.RIGHT)
add_animation(player, animationHolder["DEFAULT_UP"], false, Direction.UP)
add_animation(player, animationHolder["DEFAULT_LEFT"], false, Direction.LEFT)
add_animation(player, animationHolder["WALK_DOWN"], true, Direction.DOWN)
add_animation(player, animationHolder["WALK_RIGHT"], true, Direction.RIGHT)
add_animation(player, animationHolder["WALK_UP"], true, Direction.UP)
add_animation(player, animationHolder["WALK_LEFT"], true, Direction.LEFT)

screen:schedule_screen_position_update(player)

--- MENU
print(" > adding menu")
local buttonW = TILESIZEINPUT * 6;
local buttonH = TILESIZEINPUT * 2;
menu = screen:add_menu2(Position.RIGHT, 0, 0, buttonW*3, 0)
local text_size = 12 --button.screenPosition.h/2

function add_button(name, title, x, y)
    local button = menu:add_button2(name, x*buttonW, y*buttonH, buttonW, buttonH, 0)
    button.image = imageManager:get_image('button')
    button.text = imageManager:add_text(name, title, colors.button, text_size, buttonW*10)
end

print(" > adding buttons")
add_button("main_menu", "Main menu", 0, 0)
screen:schedule_button_update('main_menu', screen:l_nextScreen("main_menu"))
screen:schedule_button_update('main_menu', screen:l_quit())

add_button("pause", "Pause", 1, 0)
screen:schedule_button_update('pause', screen:l_pause())
help_text = imageManager:add_text("pause_text", "Paused", colors.red, 64, buttonW*10)
screen.keybinds:add_keybind("pause", KEYS.escape, screen:l_pause(), screen:l_pause())

add_button("quit", "Quit", 2, 0)
screen:schedule_button_update('quit', screen:l_quit())

add_button("+", "+", 0, 1)
screen:schedule_button_update('+', screen:l_screen_zoom(1.5))
screen.keybinds:add_keybind("zoom_in", KEYS.p, screen:l_screen_zoom(1.5))

add_button("-", "-", 1, 1)
screen:schedule_button_update('-', screen:l_screen_zoom(0.5))
screen.keybinds:add_keybind("zoom_out", KEYS.minus, screen:l_screen_zoom(0.5))

add_button("fps", "FPS", 2, 1)
screen.properties:set("showFPS", false)
screen:schedule_button_update('fps', screen:l_property_update("showFPS", not screen.properties:get_bool("showFPS")))

-- Movement
print(" > adding movement")
screen.keybinds:add_keybind("up", KEYS.w, l_player_move_up(player), l_player_move_down(player))
screen.keybinds:add_keybind("down", KEYS.s, l_player_move_down(player), l_player_move_up(player))
screen.keybinds:add_keybind("left", KEYS.a, l_player_move_left(player), l_player_move_right(player))
screen.keybinds:add_keybind("right", KEYS.d, l_player_move_right(player), l_player_move_left(player))

-- Plotting
print(" > adding plots")
screen:schedule_plot(screen:l_plot_on_level(level))


