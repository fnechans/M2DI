file(GLOB FILES *.cpp )

add_library( managers  ${FILES}  )
target_include_directories(managers PUBLIC ../Tools ../SDL_wrapper ../Basics)
