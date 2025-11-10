animantionData = {}
sprites = Sprites.new()

function create_animation(name, image_name, clips, shift, frequency, rpt)
    --[[
    frequency: how often the animation should be played
        in frames
    clips: an array of SDL_Rects that will be played in order
        corresponding to part of image
    rpt: should the animation repeat automatically
    ]]
    local animation = AnimationData.new()
    local image = imageManager:get_image(image_name)
    print("Adding animation: " .. name)
    for i = 1, #clips do
        print("    " .. name .. i)
        sprites:add(name .. i, image, clips[i], colors.none, shift)
        animation:add_sprite(sprites:at(name .. i))
    end
    animation:set_frequency(frequency, FRAMES_PER_SECOND)
    animation.rpt = rpt
    animantionData[name] = animation
    return animation
end

function add_animation(char, animation_name, properties, values)
    local _anim = animantionData[animation_name]:get_animation()
    checkers = create_checker()
    for i = 1, #properties do
        add_checker(checkers, properties[i], values[i])
    end
    char:add_animation(_anim, checkers)
end