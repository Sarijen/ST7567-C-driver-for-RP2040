cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(ST7567 C CXX)
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


option(BUILD_EXAMPLE "Name of example to build (empty disables all)" "")

if(BUILD_EXAMPLE)
  add_subdirectory(examples)
else()
  message(STATUS "Use -DBUILD_EXAMPLE=<name> to build examples")
endif()
