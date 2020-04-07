file(GLOB FILES *.cpp )

find_package(SDL2 REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(../Tools)
include_directories(../SDL_wrapper)
MESSAGE( "Library path:  ${SDL2_TTF_LIBRARIES}"   )

add_library( objects  ${FILES}  )
target_link_libraries( objects ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} -lstdc++fs )
target_include_directories( objects PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} )
