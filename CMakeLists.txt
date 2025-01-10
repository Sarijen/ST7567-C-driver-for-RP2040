cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(ST7567 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

pico_sdk_init()

add_executable(ST7567
  src/ST7567.c
)

target_link_libraries(ST7567 pico_stdlib hardware_spi hardware_gpio)

# Enable for USB debugging
#pico_enable_stdio_usb(lcd 1)

pico_add_extra_outputs(ST7567)
