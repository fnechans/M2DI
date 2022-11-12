file(GLOB FILES *.cpp )

add_library( complex  ${FILES}  )
target_link_libraries(complex PUBLIC tools wrappers basics dmgers)
target_include_directories(complex PUBLIC ${CMAKE_CURRENT_LIST_DIR})
