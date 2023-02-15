file(GLOB FILES *.cpp *h)

add_library( tools SHARED ${FILES}  )
target_include_directories(tools PUBLIC ${CMAKE_CURRENT_LIST_DIR})