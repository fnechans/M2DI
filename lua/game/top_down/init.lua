
-- Map
print(" > initializing map")


map_image = imageManager:get_image('map')
function add_sprite(name, x, y, col)
    sprites:add(name, map_image, rect_tile(x, y, 1, 1), col, Fl_Rect.new(0, 0, 0, 0))
end
add_sprite("sand", 1, 4, colors.yellow)
add_sprite("grass", 0, 0, colors.green)
add_sprite("dirt", 12, 13, colors.brown)
add_sprite("wood", 4, 5, colors.wood)
add_sprite("water", 0, 1, colors.blue)

level = screen:add_level(Position.WHOLE, rect(0, 0, 0, 0))
level:set_map(SDL_Rect.new(0, 0, 120, 0))
map = level:get_map()
blocks = level:get_blocks()
for i = -10, 10 do
    for j = -10, 10 do
        map:add_chunk(Chunk.new(i, j, "data/start.map", sprites, 16))
        blocks:add_chunk(Chunk.new(i, j, "data/startBlock2.map", sprites, 16))
    end
end

level:set_ai()

-- Animations
print(" > initializing animations")

function anim_player(name, clips, freq)
    create_animation(name, "player", clips, float_rect(-0.5, -1.5, 1, 2), freq, true)
end
anim_player("DEFAULT_DOWN", {rect_tile(0, 0, 1, 2), rect_tile(6, 0, 1, 2)}, 1)
anim_player("DEFAULT_RIGHT", {rect_tile(0, 2, 1, 2), rect_tile(6, 2, 1, 2)}, 1)
anim_player("DEFAULT_UP", {rect_tile(0, 4, 1, 2), rect_tile(6, 4, 1, 2)}, 1)
anim_player("DEFAULT_LEFT", {rect_tile(0, 6, 1, 2), rect_tile(6, 6, 1, 2)}, 1)
anim_player("WALK_DOWN", {rect_tile(0, 0, 1, 2), rect_tile(1, 0 ,1 ,2), rect_tile(2, 0, 1, 2), rect_tile(3, 0, 1, 2)}, 0.15)
anim_player("WALK_RIGHT", {rect_tile(0, 2, 1, 2), rect_tile(1, 2 ,1 ,2), rect_tile(2, 2, 1, 2), rect_tile(3, 2, 1, 2)}, 0.15)
anim_player("WALK_UP", {rect_tile(0, 4, 1, 2), rect_tile(1, 4 ,1 ,2), rect_tile(2, 4, 1, 2), rect_tile(3, 4, 1, 2)}, 0.15)
anim_player("WALK_LEFT", {rect_tile(0, 6, 1, 2), rect_tile(1, 6 ,1 ,2), rect_tile(2, 6, 1, 2), rect_tile(3, 6, 1, 2)}, 0.15)

function anim_log(name, clips, freq)
    create_animation(name, "log", clips, float_rect(-1, -1.656, 2, 2), freq, true)
end
anim_log("LOG_DEFAULT", {rect_tile(8, 0, 2, 2), rect_tile(8, 2, 2, 2), rect_tile(8, 4, 2, 2), rect_tile(8, 6, 2, 2)}, 1)
anim_log("LOG_WALK_DOWN", {rect_tile(0, 0, 2, 2), rect_tile(2, 0 ,2 ,2), rect_tile(4, 0, 2, 2), rect_tile(6, 0, 2, 2)}, 0.15)
anim_log("LOG_WALK_UP", {rect_tile(0, 2, 2, 2), rect_tile(2, 2 ,2 ,2), rect_tile(4, 2, 2, 2), rect_tile(6, 2, 2, 2)}, 0.15)
anim_log("LOG_WALK_RIGHT", {rect_tile(0, 4, 2, 2), rect_tile(2, 4 ,2 ,2), rect_tile(4, 4, 2, 2), rect_tile(6, 4, 2, 2)}, 0.15)
anim_log("LOG_WALK_LEFT", {rect_tile(0, 6, 2, 2), rect_tile(2, 6 ,2 ,2), rect_tile(4, 6, 2, 2), rect_tile(6, 6, 2, 2)}, 0.15)

-- Add chars
print(" > adding characters")
player = level:add_character("player", 9, 9, 0.8, 0.8);
-- player.mapColor = SDL_Color.new(0, 250, 0, 255)

function char_anim(char)
    add_animation(char, "DEFAULT_DOWN", {"moved", "direction"}, {false, Direction.DOWN})
    add_animation(char, "DEFAULT_RIGHT", {"moved", "direction"}, {false, Direction.RIGHT})
    add_animation(char, "DEFAULT_UP", {"moved", "direction"}, {false, Direction.UP})
    add_animation(char, "DEFAULT_LEFT", {"moved", "direction"}, {false, Direction.LEFT})
    add_animation(char, "WALK_DOWN", {"moved", "direction"}, {true, Direction.DOWN})
    add_animation(char, "WALK_RIGHT", {"moved", "direction"}, {true, Direction.RIGHT})
    add_animation(char, "WALK_UP", {"moved", "direction"}, {true, Direction.UP})
    add_animation(char, "WALK_LEFT", {"moved", "direction"}, {true, Direction.LEFT})
end

char_anim(player)

enemy = level:add_character("enemy", 2, 10, 0.8, 0.8);
enemy.speedX = 3
enemy.speedY = 3
-- enemy.mapColor = SDL_Color.new(0, 250, 0, 255)

function log_anim(char)
    add_animation(char, "LOG_DEFAULT", {"moved"}, {false})
    add_animation(char, "LOG_WALK_DOWN", {"moved", "direction"}, {true, Direction.DOWN})
    add_animation(char, "LOG_WALK_UP", {"moved", "direction"}, {true, Direction.UP})
    add_animation(char, "LOG_WALK_RIGHT", {"moved", "direction"}, {true, Direction.RIGHT})
    add_animation(char, "LOG_WALK_LEFT", {"moved", "direction"}, {true, Direction.LEFT})
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
level:add_minimap(Position.RIGHT, rect(0, buttonH*2, buttonW*3, buttonW*3))

function add_button(name, title, x, y)
    local button = menu:add_button_width_height(name,
        rect(x*buttonW, y*buttonH, buttonW, buttonH),
        buttonSpriteW, buttonSpriteH, 0
    )
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
screen:schedule_button_update('fps', screen:l_property_toggle("showFPS"))

-- Movement
print(" > adding movement")
screen.keybinds:add_keybind("up", KEYS.w, l_player_move_up(player), l_player_move_down(player))
screen.keybinds:add_keybind("down", KEYS.s, l_player_move_down(player), l_player_move_up(player))
screen.keybinds:add_keybind("left", KEYS.a, l_player_move_left(player), l_player_move_right(player))
screen.keybinds:add_keybind("right", KEYS.d, l_player_move_right(player), l_player_move_left(player))

-- Plotting
print(" > adding plots")
screen:schedule_plot(screen:l_plot_on_level(level))


