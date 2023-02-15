cmake_minimum_required(VERSION 3.7)

project(SDL2Test)
SET(CMAKE_BUILD_TYPE Debug)


SET(WLEVEL     " -Wall -Wextra")
SET(CXX11 "-std=c++11 -std=c++14 -std=c++17 ${WLEVEL} ")
IF(UNIX)
    MESSAGE( STATUS "Found UNIX System (including OS X and CygWin)" )

    SET(PLT lin)

    SET(CMAKE_CXX_FLAGS     "-g ${CXX11} ${Boost_FLAGS}  -pthread")
    SET(CMAKE_Fortran_FLAGS "-m64" )

    SET(CMAKE_CXX_FLAGS_DEBUG          "-g -O0 ${CXX11} ${Boost_FLAGS} -Wno-deprecated-register  -pthread")

    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 ${CXX11}")

    SET(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG ${CXX11}")

    SET(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG ${CXX11}")

ENDIF(UNIX)

if (MVSC OR WINDOWS OR CYGWIN OR WIN32)
    SET(CXX11 "-std:c++17")
    MESSAGE( STATUS "Found Windows System" )

    SET(CMAKE_CXX_FLAGS     "${CXX11}")
endif()

set(sdllibs SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL_gfx-static)

add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
include_directories(external/SDL/include)
add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)
include_directories(external/SDL_image)
add_subdirectory(external/SDL_ttf EXCLUDE_FROM_ALL)
include_directories(external/SDL_ttf)
add_subdirectory(external/SDL_gfx)# EXCLUDE_FROM_ALL)
include_directories(external/SDL_gfx)

if(MVSC OR WINDOWS OR CYGWIN OR WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

add_subdirectory(Tools)

add_subdirectory(SDL_wrapper)

add_subdirectory(Basics)

add_subdirectory(Damage)

add_subdirectory(Complex)

add_subdirectory(Managers)

add_subdirectory(Screens)
include_directories(Screens)

include_directories(User)


add_executable(SDL2Test main.cpp)

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(SDL2Test PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(SDL2Test
    PUBLIC  screens
                    ${sdllibs}
                    -lstdc++fs
)

# TODO: proper install!
install(TARGETS SDL2Test DESTINATION ${CMAKE_CURRENT_LIST_DIR}/install)
