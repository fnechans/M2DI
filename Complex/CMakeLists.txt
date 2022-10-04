file(GLOB FILES *.cpp )

add_library( complex  ${FILES}  )
target_include_directories(complex PUBLIC ../Tools ../SDL_wrapper ../Basics ../Damage)
