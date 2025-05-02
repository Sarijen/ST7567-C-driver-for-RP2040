# ST7567 driver for RPi Pico (RP2040)
This driver allows you to control a ST7567 based LCD using SPI with the RP2040 MCU.
It includes functions for initializing the display, drawing graphics and pixels using a framebuffer, dimming the backlight and more.

# Features
- Clearable and easily writable framebuffer 
- Drawing lines, rectangles, circles and individual pixels
- Drawing built-in or custom fonts
- Displaying bitmap images
- PWM dimmable backlight

# Showcase
<img src="images/showcase.gif" width="300"/>

###### showcase.c example code

# Wiring Diagram
- Wiring doesn't have to be exactly the same as shown below, just ensure you use `correct` SPI pins for your chosen instance (`SPI0` or `SPI1`)
- `CS` pin can be connected to any GPIO pin you want, assuming you specify it correctly in your code


<img src="images/wiringDiagram.png" width="550"/>

| Pico | Display |
| :------: |:----:|
| 3V3(OUT) | 3V3 |
| GND | GND |
| SPI TX | SDI |
| SPI SCK | SCK |
| Any GPIO | DC |
| Any GPIO | CS |
| Any GPIO | RST |
| GND | NC/LED |

Connect `NC/LED` pin to `GND` to enable backlight.

For PWM dimming use a transistor/MOSFET, connecting base to `any GPIO`, Collector/Emitter to `GND` and `NC/LED`.

# Usage

## Built-in example code
1. Make sure you have [pico SDK](https://github.com/raspberrypi/pico-sdk) installed and working on your machine
2. `git clone` this repo and set `PICO_SDK_PATH` in it
3. Choose 1 example by uncommenting it in CMakeLists.txt
4. build and compile using

    ```
    mkdir build
    cd build
    cmake ..
    make
    ```

5. Flash the ST7567_example.uf2 located in the same directory. 

## Integrating into your codebase
1. `git clone` this repo inside your project

2. modify your CMakeLists.txt 

    add the library directory using
    ``` 
    add_subdirectory(ST7567-C-driver-for-RP2040)
    ```
    add `ST7567` keyword to target_link_libraries

3. include header in your .c file
    ```
    #include "ST7567.h"
    ```

4. Properly use the library, check `examples` folder for [API reference](examples/README.md) and proper usage.
