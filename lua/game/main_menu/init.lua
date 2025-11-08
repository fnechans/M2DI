menu = screen:add_menu2(Position.CENTER, 0, TILESIZEINPUT*2, TILESIZEINPUT*12, 0)

local buttonW = TILESIZEINPUT * 12;
local buttonH = TILESIZEINPUT * 4;
local buttonSpriteW = TILESIZEINPUT*2
local buttonSpriteH = TILESIZEINPUT

local button = menu:add_button_width_height('topdown', 
    rect(0, buttonH, buttonW, buttonH),
    buttonSpriteW, buttonSpriteH, 0
)
button.image = imageManager:get_image('button')
local text_size = 32 --button.screenPosition.h/2
button.text = imageManager:add_text("topdown", "Top-down", colors.button, text_size, buttonW*10)

local button = menu:add_button_width_height('editor',
    rect(0, buttonH*3, buttonW, buttonH),
    buttonSpriteW, buttonSpriteH, 0
)
button.image = imageManager:get_image('button')
local text_size = 32 --button.screenPosition.h/2
button.text = imageManager:add_text("editor", "Editor", colors.button, text_size, buttonW*10)

local button = menu:add_button_width_height("quit",
    rect(0, buttonH*5, buttonW, buttonH),
    buttonSpriteW, buttonSpriteH, 0
)
button.image = imageManager:get_image('button')
button.text = imageManager:add_text("quit", "Quit", colors.button, text_size, buttonW*10)

screen:schedule_button_update('quit', screen:l_quit())
screen:schedule_button_update('topdown', screen:l_nextScreen("top_down"))
screen:schedule_button_update('topdown', screen:l_quit())
screen:schedule_button_update('editor', screen:l_nextScreen("editor"))
screen:schedule_button_update('editor', screen:l_quit())
