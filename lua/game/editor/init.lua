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

local buttonW = TILESIZEINPUT * 8;
local buttonH = TILESIZEINPUT * 3;

--- LEVEL
level = screen:add_level(Position.LEFT_FILL, rect(0, 0, buttonW*3, 0))

level:set_map()
map = level:get_map()
map:blank_map(1000, 1000)

player = level:add_character("player", 9, 9, 0.1, 0.1);
player.speedX = 24
player.speedY = 24
--- player.doPlot = false
screen:schedule_screen_position_update(player)

--- MENU
print(" > adding menu")
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
screen:schedule_button_update('fps', screen:l_property_update("showFPS", not screen.properties:get_bool("showFPS")))

function add_tile_button(name, x, y)
    local button = menu:add_button_clip(name,
        rect(x*4*TILESIZEINPUT, y*4*TILESIZEINPUT+buttonH*2, TILESIZEINPUT*4, TILESIZEINPUT*4),
        sprites:at(name).clip, 0)
    button.image = sprites:at(name).image
    screen:schedule_button_update(name, screen:l_property_update("tile", name))
end

screen.properties:set("tile", "sand")
screen:schedule_tile_click_update(
    screen:l_update_tile_from_sprite(
        sprites, screen.properties:get_string("tile")
    )
)

add_tile_button("sand", 0, 0)
add_tile_button("grass", 1, 0)
add_tile_button("dirt", 2, 0)
add_tile_button("wood", 3, 0)
add_tile_button("water", 4, 0)



-- Movement
print(" > adding movement")
screen.keybinds:add_keybind("up", KEYS.w, l_player_move_up(player), l_player_move_down(player))
screen.keybinds:add_keybind("down", KEYS.s, l_player_move_down(player), l_player_move_up(player))
screen.keybinds:add_keybind("left", KEYS.a, l_player_move_left(player), l_player_move_right(player))
screen.keybinds:add_keybind("right", KEYS.d, l_player_move_right(player), l_player_move_left(player))

-- Plotting
print(" > adding plots")
screen:schedule_plot(screen:l_plot_on_level(level))

