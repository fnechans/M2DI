file(GLOB FILES *.cpp)

add_library( tools astar.h base.h base.cpp tools.h tools.cpp)# ${FILES}  )
target_include_directories(tools PUBLIC ${CMAKE_CURRENT_LIST_DIR})