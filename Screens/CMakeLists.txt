file(GLOB FILES *.cpp )

add_library( screens  ${FILES}  )
target_link_libraries(screens basics wrappers dmgers complex tools managers)
target_include_directories(screens PUBLIC ${CMAKE_CURRENT_LIST_DIR})