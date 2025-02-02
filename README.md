# ST7567 driver for the RPi Pico (RP2040)
This driver allows you to control an LCD using SPI with the RP2040 MCU.
It includes functions for initializing the display and drawing graphics using a framebuffer.


## Features
- Clearable and easily writable Framebuffer 
- Drawing shape functions for lines, rectangles, circles and even individual pixels
- Drawing various fonts (To be done)
- Displaying images
- PWM dimmable backlight


## Usage
(To be done)


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
