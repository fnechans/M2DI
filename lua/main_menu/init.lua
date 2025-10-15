menu = screen:add_menu2(Position.CENTER, 0, TILESIZEINPUT*2, TILESIZEINPUT*12, 0)
local buttonW = TILESIZEINPUT * 12;
local buttonH = TILESIZEINPUT * 4;
local button = menu:add_button2('topdown', 0, buttonH, buttonW, buttonH, 0)
button.image = imageManager:get_image('button')
local text_size = 32 --button.screenPosition.h/2
button.text = imageManager:add_text("topdown", "Top-down", colors.button, text_size, buttonW*10)

local button = menu:add_button2("quit", 0, buttonH*5, buttonW, buttonH, 0)
button.image = imageManager:get_image('button')
button.text = imageManager:add_text("quit", "Quit", colors.button, text_size, buttonW*10)

screen:schedule_button_update('quit', screen:l_quit())
screen:schedule_button_update('topdown', screen:l_nextScreen("top_down"))
screen:schedule_button_update('topdown', screen:l_quit())
