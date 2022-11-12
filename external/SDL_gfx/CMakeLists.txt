cmake_minimum_required (VERSION 3.7)
project (SDL_gfx)
set (SDL_gfx_VERSION_MAJOR 3)
set (SDL_gfx_VERSION_MINOR 0)

set (CMAKE_MODULE_PATH "cmake/modules")
find_package (SDL2 REQUIRED)

include_directories (
	"${SDL2_INCLUDE_DIR}"
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

target_link_libraries (SDL_gfx
	${SDL2_LIBRARY}
	)

install (TARGETS SDL_gfx DESTINATION lib)
install (FILES ${INCLUDE_FILES} DESTINATION include/SDL)
