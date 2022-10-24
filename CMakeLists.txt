cmake_minimum_required(VERSION 3.7)

project(SDL2Test)
SET(CMAKE_BUILD_TYPE Debug)


SET(WLEVEL     " -Wall -Wextra")
SET(CXX11 "-std=c++11 -std=c++14 -std=c++17 ${WLEVEL} ")
IF(UNIX)
    MESSAGE( STATUS "Found UNIX System (including OS X and CygWin)" )

    SET(PLT lin)

    SET(CMAKE_C_FLAGS       "-g ${CXX11} ${Boost_FLAGS} "  )
    SET(CMAKE_CXX_FLAGS     "-g ${CXX11} ${Boost_FLAGS}  -pthread")
    SET(CMAKE_Fortran_FLAGS "-m64" )

    SET(CMAKE_CXX_FLAGS_DEBUG          "-g -O0 ${CXX11} ${Boost_FLAGS} -Wno-deprecated-register  -pthread")
    SET(CMAKE_C_FLAGS_DEBUG            "-g -O0 ${CXX11} ${Boost_FLAGS} ")

    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 ${CXX11}")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO   "-g -O2 ${CXX11}")

    SET(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG ${CXX11}")
    SET(CMAKE_C_FLAGS_RELEASE          "-O4 -DNDEBUG ${CXX11}")

    SET(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG ${CXX11}")
    SET(CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG ${CXX11}")

ENDIF(UNIX)

# STATIC stuff (Windows)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(BUILD_FOR_WIN TRUE)
        SET(PLT win)
endif()
option(STATIC_BUILD "Build a static binary." ${BUILD_FOR_WIN})
if (WIN32)
    MESSAGE( STATUS "Found UNIX System (including OS X and CygWin)" )

    SET(PLT win)

    SET(CMAKE_C_FLAGS       "-g ${CXX11} ${Boost_FLAGS} "  )
    SET(CMAKE_CXX_FLAGS     "-g ${CXX11} ${Boost_FLAGS}")
    SET(CMAKE_Fortran_FLAGS "-m64" )

    SET(CMAKE_CXX_FLAGS_DEBUG          "-g -O0 ${CXX11} ${Boost_FLAGS} -Wno-deprecated-register ")
    SET(CMAKE_C_FLAGS_DEBUG            "-g -O0 ${CXX11} ${Boost_FLAGS} ")

    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 ${CXX11}")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO   "-g -O2 ${CXX11}")

    SET(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG ${CXX11}")
    SET(CMAKE_C_FLAGS_RELEASE          "-O4 -DNDEBUG ${CXX11}")

    SET(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG ${CXX11}")
    SET(CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG ${CXX11}")
endif()

if (STATIC_BUILD)
        set(CMAKE_EXE_LINKER_FLAGS "-static")
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" CONFIG)
        set(BUILD_SHARED_LIBS OFF)
        message(STATUS "making static build")
endif()

message(platform ${PLT})
find_package(PkgConfig REQUIRED)
set( ENV{PKG_CONFIG_PATH} "${CMAKE_SOURCE_DIR}/SDL/${PLT}/SDL2/lib/pkgconfig" )
set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${CMAKE_SOURCE_DIR}/SDL/${PLT}/SDL2_image/lib/pkgconfig" )
set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${CMAKE_SOURCE_DIR}/SDL/${PLT}/SDL2_ttf/lib/pkgconfig" )
set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${CMAKE_SOURCE_DIR}/SDL/${PLT}/SDL2_gfx/lib/pkgconfig" )
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2 SDL2_ttf SDL2_image SDL2_gfx)


include(CMakePrintHelpers)
if (BUILD_FOR_WIN)
    SET(SDL2_INCLUDE_DIRS  "/home/filip/Documents/projects/M2DI/SDL/win/SDL2/include;/home/filip/Documents/projects/M2DI/SDL/win/SDL2/include/SDL2;/home/filip/Documents/projects/M2DI/SDL/win/SDL2_image/include;/home/filip/Documents/projects/M2DI/SDL/win/SDL2_ttf/include")
    SET(SDL2_LIBRARY_DIRS  "/home/filip/Documents/projects/M2DI/SDL/win/SDL2/lib;/home/filip/Documents/projects/M2DI/SDL/win/SDL2_image/lib;/home/filip/Documents/projects/M2DI/SDL/win/SDL2_ttf/lib")
    link_libraries("-mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32  -lusp10 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc  -lsetupapi -lhid -lrpcrt4")
    #-lmingw32 -lSDL2main -lSDL2
endif()
include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})

cmake_print_variables(SDL2_INCLUDE_DIRS)
cmake_print_variables(SDL2_LIBRARIES)
cmake_print_variables(SDL2_LINK_LIBRARIES)
cmake_print_variables(SDL2_LIBRARY_DIRS)
add_subdirectory(SDL_wrapper)

add_subdirectory(Tools)

add_subdirectory(Basics)

add_subdirectory(Damage)

add_subdirectory(Complex)

add_subdirectory(Screens)
include_directories(Screens)

include_directories(User)


add_executable(SDL2Test main.cpp)
target_link_libraries(SDL2Test
    PUBLIC  wrappers tools basics complex dmgers screens
                    ${SDL2_LIBRARIES}
                    #PkgConfig::SDL2
                    -lstdc++fs
)
target_compile_options(SDL2Test PUBLIC ${SDL2_CFLAGS_OTHER})
