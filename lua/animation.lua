animantionData = {}
sprites = Sprites.new()

function create_animation(name, frequency, clips ,rpt)
    --[[
    frequency: how often the animation should be played
        in frames
    clips: an array of SDL_Rects that will be played in order
        corresponding to part of image
    rpt: should the animation repeat automatically
    ]]
    local animation = AnimationData.new()
    animation:set_frequency(frequency, FRAMES_PER_SECOND)
    animation.clips = clips
    animation.rpt = rpt
    animantionData[name] = animation
    return animation
end

function add_animation(char, image_name, animation_name, properties, values, shift)
    _anim = animantionData[animation_name]:get_animation()
    _anim.image = imageManager:get_image(image_name)
    checkers = create_checker()
    for i = 1, #properties do
        add_checker(checkers, properties[i], values[i])
    end
    char:add_animation(_anim, checkers, shift)
end