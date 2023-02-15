file(GLOB FILES *.cpp )

add_library( dmgers  ${FILES}  )
target_link_libraries(dmgers tools wrappers basics)
target_include_directories(dmgers PUBLIC ${CMAKE_CURRENT_LIST_DIR})
