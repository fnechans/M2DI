file(GLOB FILES *.cpp )

add_library( basics  ${FILES}  )
target_include_directories(basics PUBLIC ../Tools ../SDL_wrapper)
