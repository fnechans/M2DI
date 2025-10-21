function script_path()
   local str = debug.getinfo(2, "S").source:sub(2)
   return str:match("(.*/)") or "./"
end

function float_rect(x, y, w, h)
    return Fl_Rect.new(x, y, w, h)
end

function rect(x, y, w, h)
    return SDL_Rect.new(x, y, w, h)
end

function rect_tile(x, y, w, h)
    return SDL_Rect.new(int(x*TILESIZEINPUT), int(y*TILESIZEINPUT), int(w*TILESIZEINPUT), int(h*TILESIZEINPUT))
end

function col(r, g, b, a)
    return SDL_Color.new(r, g, b, a)
end

function int(float)
    return math.floor(float)
end