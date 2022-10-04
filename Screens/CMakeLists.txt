file(GLOB FILES *.cpp )

add_library( screens  ${FILES}  )
target_include_directories(screens PUBLIC  ../Basics ../SDL_wrapper ../Objects ../Damage ../Complex ../Tools)

