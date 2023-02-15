file(GLOB FILES *.cpp)


add_library( wrappers   ${FILES}  )
target_link_libraries(wrappers tools ${sdllibs})
target_include_directories(wrappers PUBLIC ${CMAKE_CURRENT_LIST_DIR})
