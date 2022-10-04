file(GLOB FILES *.cpp )


add_library( wrappers   ${FILES}  )
target_include_directories(wrappers PUBLIC ../Tools)
