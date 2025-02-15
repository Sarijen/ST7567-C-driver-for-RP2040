# ST7567 driver for the RPi Pico (RP2040)
This driver allows you to control an LCD using SPI with the RP2040 MCU.
It includes functions for initializing the display and drawing graphics using a framebuffer.


## Features
- Clearable and easily writable Framebuffer 
- Drawing shape functions for lines, rectangles, circles and even individual pixels
- Drawing various fonts (To be done)
- Displaying images
- PWM dimmable backlight

## Wiring Diagram

<img src="images/wiringDiagram.png" width="500"/>

| Pico Pin | Display Pin  |
| :------: |:----:|
| 3V3(OUT) | 3V3 |
| GND | GND |
| SPI TX | SDI |
| SPI SCK | SCK |
| Any GPIO | DC |
| Any GPIO | CS |
| Any GPIO | RST |
| GND | NC/LED |

Connect NC or LED pin to ground to enable backlight, use a resistor to adjust the brightness.

## Usage

### Built-in examples
1. Make sure you have [pico SDK](https://github.com/raspberrypi/pico-sdk) installed and working on your machine
2. Run these commands
```
git clone https://github.com/Sarijen/ST7567-C-driver-for-RP2040
cd ST7567-C-driver-for-RP2040
mkdir build
cmake ..
make
```
3. Flash the .uf2 file onto the RP2040 

### Integrating into your codebase
1. TO-DO
