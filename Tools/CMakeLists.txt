file(GLOB FILES *.cpp )

find_package(SDL2 REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

MESSAGE( "Library path:  ${SDL2_TTF_LIBRARIES}"   )
include_directories(../SDL_wrapper)

add_library( tools  ${FILES}  )
target_link_libraries( tools ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} -lstdc++fs )
target_include_directories( tools PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} )
