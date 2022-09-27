cmake_minimum_required(VERSION 3.7)

project(SDL2Test)
SET(CMAKE_BUILD_TYPE Debug)

SET(WLEVEL     " -Wall -Wextra")
SET(CXX11 "-std=c++11 -std=c++14 -std=c++17 ${WLEVEL} ")
IF(UNIX)
    MESSAGE( STATUS "Found UNIX System (including OS X and CygWin)" )
    ADD_DEFINITIONS(${ROOT_COMPILE_FLAGS})


    SET(CMAKE_C_FLAGS       "-g ${CXX11} ${Boost_FLAGS} "  )
    SET(CMAKE_CXX_FLAGS     "-g ${CXX11} ${Boost_FLAGS} ")
    SET(CMAKE_Fortran_FLAGS "-m64" )

    SET(CMAKE_CXX_FLAGS_DEBUG          "-g -O0 ${CXX11} ${Boost_FLAGS} -Wno-deprecated-register ")
    #SET(CMAKE_CXX_FLAGS_DEBUG          "-g -O0 ${CXX11} ${Boost_FLAGS} -Wno-deprecated-register -Wall")
    SET(CMAKE_C_FLAGS_DEBUG            "-g -O0 ${CXX11} ${Boost_FLAGS} ")

    SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -O2 ${CXX11}")
    SET(CMAKE_C_FLAGS_RELWITHDEBINFO   "-g -O2 ${CXX11}")

    SET(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG ${CXX11}")
    SET(CMAKE_C_FLAGS_RELEASE          "-O4 -DNDEBUG ${CXX11}")

    SET(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG ${CXX11}")
    SET(CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG ${CXX11}")

    message( BOOST ${Boost_LIBRARIES} )
ENDIF(UNIX)

add_subdirectory(SDL_wrapper)
include_directories(SDL_wrapper)
link_directories(SDL_wrapper)

add_subdirectory(Tools)
include_directories(Tools)
link_directories(Tools)

add_subdirectory(Objects)
include_directories(Objects)
link_directories(Objects)

add_subdirectory(Damage)
include_directories(Damage)
link_directories(Damage)

add_subdirectory(Complex)
include_directories(Complex)
link_directories(Complex)

add_subdirectory(Screens)
include_directories(Screens)
link_directories(Screens)

include_directories(User)

add_executable(SDL2Test main.cpp)
target_link_libraries(SDL2Test PUBLIC wrappers tools objects complex dmgers screens)
