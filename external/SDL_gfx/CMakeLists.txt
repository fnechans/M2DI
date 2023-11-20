cmake_minimum_required (VERSION 3.7)
project (SDL_gfx)
set (SDL_gfx_VERSION_MAJOR 3)
set (SDL_gfx_VERSION_MINOR 0)

#set (CMAKE_MODULE_PATH "cmake/modules")
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#find_package (SDL2 REQUIRED)
set(SDL_REQUIRED_VERSION 2.0.9)
set(sdl2_target_name SDL2::SDL2)
sdl_find_sdl2(${sdl2_target_name} ${SDL_REQUIRED_VERSION})

include_directories (
	#"${SDL2_INCLUDE_DIR}"
    "${CMAKE_CURRENT_LIST_DIR}/../SDL/include"
	)

file(GLOB SOURCE_FILES *.c)
file(GLOB INCLUDE_FILES *.h)

add_library (SDL_gfx SHARED
	${SOURCE_FILES}
	${INCLUDE_FILES}
	)
add_library (SDL_gfx-static STATIC
	${SOURCE_FILES}
	${INCLUDE_FILES}
	)

#target_link_libraries (SDL_gfx
#	${SDL2_LIBRARY}
#	)
target_link_libraries(SDL_gfx PRIVATE ${sdl2_target_name})

install (TARGETS SDL_gfx DESTINATION lib)
install (FILES ${INCLUDE_FILES} DESTINATION include/SDL)

add_library(SDL2::SDL_gfx ALIAS SDL_gfx)
if(TARGET SDL_gfx-static AND NOT TARGET SDL_gfx)
  add_library(SDL2::SDL_gfx ALIAS SDL_gfx-static)
endif()
