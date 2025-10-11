project('SDL2Test', 'cpp', 'c', default_options : 
    ['default_library=shared',
     'cpp_std=c++23',
   #  'warning_level=3',
     'buildtype=debug',
    ]
)
cmake_build_type = 'Debug'
cmake_install_prefix =  '/install'
wlevel = ' -Wall -Wextra'

root_inc = include_directories('.')
# include_directories('Screens')
# include_directories('User')
sdl2 = dependency('sdl2')
# sdl2 = declare_dependency(
#   dependencies : sdl2_raw,               # link flag (‑lSDL2, .lib, …)
#   include_directories : sdl2_raw.include_directories(),
# )
sdl2image = dependency('sdl2_image')
sdl2ttf = dependency('sdl2_ttf')
sol2proj = subproject('sol2')
sol2 = sol2proj.get_variable('sol2_dep')
sdl = declare_dependency(dependencies: [sdl2, sdl2image, sdl2ttf])

subdir('Tools')
subdir('SDL_wrapper')
subdir('Basics')
subdir('Damage')
subdir('Complex')
subdir('Managers')
subdir('Screens')

#SDL2Test_exe = executable('SDL2Test', 'main.cpp', dependencies: [sdl], link_with: [tools_lib, wrappers_lib, basics_lib, dmgers_lib, complex_lib, managers_lib, screens_lib], include_directories: [root_inc])

run_lua = executable('run_lua', 'run_lua.cpp', dependencies: [sdl, sol2], link_with: [tools_lib, wrappers_lib, basics_lib, dmgers_lib, complex_lib, managers_lib, screens_lib], include_directories: [root_inc])
