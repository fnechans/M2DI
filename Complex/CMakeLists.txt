file(GLOB FILES *.cpp )

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

include_directories(../Tools)
include_directories(../SDL_wrapper)
include_directories(../Objects)

add_library( complex  ${FILES}  )
target_link_libraries( complex objects ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES} -lstdc++fs )
target_include_directories( complex PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} )
