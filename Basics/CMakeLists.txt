file(GLOB FILES *.cpp )

add_library( basics  ${FILES}  )
target_link_libraries(basics tools wrappers)
target_include_directories(basics PUBLIC ${CMAKE_CURRENT_LIST_DIR})
