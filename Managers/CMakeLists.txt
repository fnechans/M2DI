file(GLOB FILES *.cpp *.hpp)

add_library( managers  ${FILES}  )
target_link_libraries(managers PUBLIC tools wrappers basics)
target_include_directories(managers PUBLIC ${CMAKE_CURRENT_LIST_DIR})
set_target_properties(managers PROPERTIES LINKER_LANGUAGE CXX)