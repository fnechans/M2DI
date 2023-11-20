file(GLOB FILES *.cpp *.hpp)

add_library( managers INTERFACE ${FILES}  )
target_link_libraries(managers INTERFACE tools wrappers basics)
target_include_directories(managers INTERFACE  ${CMAKE_CURRENT_LIST_DIR})
#set_target_properties(managers PROPERTIES LINKER_LANGUAGE CXX)