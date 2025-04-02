cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(ST7567 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_library(ST7567 STATIC
  src/ST7567.c
  src/fonts.c
)
target_include_directories(ST7567 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(
  ST7567
  pico_stdlib
  hardware_spi
  hardware_gpio
  hardware_pwm
  hardware_clocks
)

add_executable(ST7567_example
  src/ST7567.c
  src/fonts.c
# examples/animation.c
 examples/basics.c
# examples/beta_example.c
# examples/fonts.c
# examples/digital_clock.c
# examples/shapes.c
# examples/images.c
)

target_link_libraries(
  ST7567_example
  pico_stdlib
  hardware_spi
  hardware_gpio
  hardware_pwm
  hardware_rtc
)

# Enable for USB debugging
# pico_enable_stdio_usb(ST7567_example 1)

pico_add_extra_outputs(ST7567_example)
