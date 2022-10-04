file(GLOB FILES *.cpp )

add_library( dmgers  ${FILES}  )
target_include_directories(dmgers PUBLIC ../Tools ../Complex ../SDL_wrapper ../Basics)
