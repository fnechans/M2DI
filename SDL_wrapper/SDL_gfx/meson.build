#project('SDL_gfx', 'c')
#sdlgfx = library('SDL_gfx', 'e.c', 'SDL2_gfxPrimitives.c', 'SDL2_imageFilter.c', 'SDL2_rotozoom.c')
# Header search path for anything that includes A
gfx_inc = include_directories('.')

# All source files belonging to A
gfx_sources = files(
 'SDL2_gfxPrimitives.c', 'SDL2_imageFilter.c', 'SDL2_rotozoom.c' 
)
sdl2 = dependency('sdl2')
sdl2image = dependency('sdl2_image')
sdl2ttf = dependency('sdl2_ttf')

sdlgfx = library('sdlgfx', gfx_sources,
  include_directories : [gfx_inc],
  dependencies: [sdl],
  install : true)   # installs liba.a and the headers if you ever package