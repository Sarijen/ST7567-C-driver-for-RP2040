if(BUILD_EXAMPLE STREQUAL "bare_minimum")
  add_executable(ST7567_example  bare_minimum.c)

elseif(BUILD_EXAMPLE STREQUAL "showcase")
  add_executable(ST7567_example showcase.c)

elseif(BUILD_EXAMPLE STREQUAL "digital_clock")
  add_executable(ST7567_example digital_clock.c)

elseif(BUILD_EXAMPLE STREQUAL "custom_fonts")
  add_executable(ST7567_example
    custom_fonts/custom_fonts.c
    custom_fonts/seamless_segments.c
    custom_fonts/x15_segments.c
  )

elseif(BUILD_EXAMPLE STREQUAL "beta_example")
  message(WARNING "'beta_example' is for personal testing only and will never be included in remote repo!")
  add_executable(ST7567_example beta_example.c)

else()
  message(FATAL_ERROR "Unknown example '${BUILD_EXAMPLE}'")
endif()

target_link_libraries(ST7567_example PRIVATE ST7567)
pico_add_extra_outputs(ST7567_example)
