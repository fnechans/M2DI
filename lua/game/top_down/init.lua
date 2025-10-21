
-- Map
print(" > initializing map")


map_image = imageManager:get_image('map')
function add_sprite(name, x, y, col, obs)
    sprites:add(name, map_image, rect_tile(x, y, 1, 1), col, obs)
end
add_sprite("sand", 1, 4, colors.yellow, true)
add_sprite("grass", 0, 0, colors.green, true)
add_sprite("dirt", 12, 13, colors.brown, true)
add_sprite("wood", 4, 5, colors.wood, true)
add_sprite("water", 0, 1, colors.blue, false)

level = screen:add_level()
level:set_map(SDL_Rect.new(0, 0, 120, 0))
map = level:get_map()
map:import_map("data/start.map", sprites)

blocks = level:get_blocks()
blocks:import_map("data/startBlock2.map", sprites)

level:set_ai()

-- Animations
print(" > initializing animations")

create_animation("DEFAULT_DOWN", 1, {rect_tile(0, 0, 1, 2), rect_tile(6, 0, 1, 2)}, true)
create_animation("DEFAULT_RIGHT", 1, {rect_tile(0, 2, 1, 2), rect_tile(6, 2, 1, 2)}, true)
create_animation("DEFAULT_UP", 1, {rect_tile(0, 4, 1, 2), rect_tile(6, 4, 1, 2)}, true)
create_animation("DEFAULT_LEFT", 1, {rect_tile(0, 6, 1, 2), rect_tile(6, 6, 1, 2)}, true)


create_animation("WALK_DOWN", 0.15, {
    rect_tile(0, 0, 1, 2), rect_tile(1, 0 ,1 ,2), rect_tile(2, 0, 1, 2), rect_tile(3, 0, 1, 2)}, true)
create_animation("WALK_RIGHT", 0.15, {
    rect_tile(0, 2, 1, 2), rect_tile(1, 2 ,1 ,2), rect_tile(2, 2, 1, 2), rect_tile(3, 2, 1, 2)}, true)
create_animation("WALK_UP", 0.15, {
    rect_tile(0, 4, 1, 2), rect_tile(1, 4 ,1 ,2), rect_tile(2, 4, 1, 2), rect_tile(3, 4, 1, 2)}, true)
create_animation("WALK_LEFT", 0.15, {
    rect_tile(0, 6, 1, 2), rect_tile(1, 6 ,1 ,2), rect_tile(2, 6, 1, 2), rect_tile(3, 6, 1, 2)}, true)

create_animation("LOG_DEFAULT", 1, {rect_tile(0, 0, 2, 2), rect_tile(10, 0, 2, 2)}, true)
create_animation("LOG_WALK_DOWN", 0.15, {
    rect_tile(0, 0, 2, 2), rect_tile(2, 0 ,2 ,2), rect_tile(4, 0, 2, 2), rect_tile(6, 0, 2, 2)}, true)
create_animation("LOG_WALK_UP", 0.15, {
    rect_tile(0, 2, 2, 2), rect_tile(2, 2 ,2 ,2), rect_tile(4, 2, 2, 2), rect_tile(6, 2, 2, 2)}, true)
create_animation("LOG_WALK_RIGHT", 0.15, {
    rect_tile(0, 4, 2, 2), rect_tile(2, 4 ,2 ,2), rect_tile(4, 4, 2, 2), rect_tile(6, 4, 2, 2)}, true)
create_animation("LOG_WALK_LEFT", 0.15, {
    rect_tile(0, 6, 2, 2), rect_tile(2, 6 ,2 ,2), rect_tile(4, 6, 2, 2), rect_tile(6, 6, 2, 2)}, true)


-- Add chars
print(" > adding characters")
player = level:add_character("player", 9, 9, 0.8, 0.8);
player.image = imageManager:get_image('player')
player.mapColor = SDL_Color.new(0, 250, 0, 255)

function char_anim(char)
    shift = float_rect(-0.5, -1.5, 1, 2)
    add_animation(char, "player", "DEFAULT_DOWN", {"moved", "direction"}, {false, Direction.DOWN}, shift)
    add_animation(char, "player", "DEFAULT_RIGHT", {"moved", "direction"}, {false, Direction.RIGHT}, shift)
    add_animation(char, "player", "DEFAULT_UP", {"moved", "direction"}, {false, Direction.UP}, shift)
    add_animation(char, "player", "DEFAULT_LEFT", {"moved", "direction"}, {false, Direction.LEFT}, shift)
    add_animation(char, "player", "WALK_DOWN", {"moved", "direction"}, {true, Direction.DOWN}, shift)
    add_animation(char, "player", "WALK_RIGHT", {"moved", "direction"}, {true, Direction.RIGHT}, shift)
    add_animation(char, "player", "WALK_UP", {"moved", "direction"}, {true, Direction.UP}, shift)
    add_animation(char, "player", "WALK_LEFT", {"moved", "direction"}, {true, Direction.LEFT}, shift)
end

char_anim(player)

enemy = level:add_character("enemy", 2, 10, 0.8, 0.8);
enemy.image = imageManager:get_image('log')
enemy.mapColor = SDL_Color.new(0, 250, 0, 255)

function log_anim(char)
    shift = float_rect(-1, -1.5, 2, 2)
    add_animation(char, "log", "LOG_DEFAULT", {"moved"}, {false}, shift)
    add_animation(char, "log", "LOG_WALK_DOWN", {"moved", "direction"}, {true, Direction.DOWN}, shift)
    add_animation(char, "log", "LOG_WALK_UP", {"moved", "direction"}, {true, Direction.UP}, shift)
    add_animation(char, "log", "LOG_WALK_RIGHT", {"moved", "direction"}, {true, Direction.RIGHT}, shift)
    add_animation(char, "log", "LOG_WALK_LEFT", {"moved", "direction"}, {true, Direction.LEFT}, shift)
end
log_anim(enemy)
enemy:set_target(player)

screen:schedule_screen_position_update(player)

--- MENU
print(" > adding menu")
local buttonW = TILESIZEINPUT * 8;
local buttonH = TILESIZEINPUT * 3;
local buttonSpriteW = TILESIZEINPUT*2
local buttonSpriteH = TILESIZEINPUT
local text_size = TILESIZEINPUT*2
menu = screen:add_menu2(Position.RIGHT, 0, 0, buttonW*3, 0)

--- minimap under the menu
level:add_minimap(Position.RIGHT, rect_tile(0, buttonH*2, buttonW*3, buttonW*3))

function add_button(name, title, x, y)
    local button = menu:add_button2(name, x*buttonW, y*buttonH, buttonW, buttonH, buttonSpriteW, buttonSpriteH, 0)
    button.image = imageManager:get_image('button')
    button.text = imageManager:add_text(name, title, colors.button, text_size, buttonW*10)
end

print(" > adding buttons")
add_button("main_menu", "Menu", 0, 0)
screen:schedule_button_update('main_menu', screen:l_nextScreen("main_menu"))
screen:schedule_button_update('main_menu', screen:l_quit())

add_button("pause", "Pause", 1, 0)
screen:schedule_button_update('pause', screen:l_pause())
help_text = imageManager:add_text("pause_text", "Paused", colors.red, 64, buttonW*10)
screen.keybinds:add_keybind("pause", KEYS.escape, screen:l_pause(), screen:l_pause())

add_button("quit", "Quit", 2, 0)
screen:schedule_button_update('quit', screen:l_quit())

add_button("+", "+", 0, 1)
screen:schedule_button_update('+', screen:l_screen_zoom(2))
screen.keybinds:add_keybind("zoom_in", KEYS.p, screen:l_screen_zoom(2))

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


