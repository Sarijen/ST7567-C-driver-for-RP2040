# API Reference

### void lcd_spi_init(spi_inst_t* spi_id, uint8_t MOSI, uint8_t SCLK, uint8_t DC, uint8_t CS, uint8_t RST, uint16_t frequency)
Inits SPI for the LCD, always required.  

spi_id - SPI instance (SPI0 or SPI1)  
frequency in `kHz`  
I was able to run it as low as `1kHz` and as high as `66MHz`! If you want to be safe stay below 20MHz, im not responsible for any damage!  

### lcd_init()
Inits the display, always required.

### void lcd_reset()
Hardware resets the display.

### void lcd_set_contrast(uint8_t RR_value, uint8_t EV_value)
Sets the display contrast  
1st argument (Regulation Ratio) changes contrast a lot, max value: `7`.  
2nd argument (Eletronic Volume) changes it a little, max value: `63`.  
For e.g. RR = 6, EV = 3 will have higher contrast than RR = 3, EV = 50.  
`Recommended values: RR = 4, EV = 31.`

### void lcd_clear_screen()
Fills the entire display with 0s, does NOT touch the framebuffer.  

### void lcd_clear_buffer()
Fills the entire buffer with 0s.  

### void lcd_display()
Sends the entire buffer to the display AKA "Refreshes it"
The stuff you drew will never be displayed until you call this function!!!

## Graphics

> Note that "draw" functions only writes pixels to the buffer, you always have to call lcd_display() to actually see what you drew.

> `uint8_t value` in draw functions represent "color" to draw. 0 = white ("erases pixels"), anything other = black ("draws pixels").

### void lcd_draw_pixel(uint8_t x, uint8_t y, uint8_t value)
Writes a single pixel at `x` `y` to the buffer.

### void lcd_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value)
Draws a line from `x1  y1` to `x2 y2`, works in all 4 quadrants, uses Bresenham's algorithm

### void lcd_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t value)
Draws an empty rectangle at `x y` with `width` and `height`

### void lcd_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t value)
Draws a fully filled rectangle at `x y` with `width` and `height` 

### void lcd_draw_image(uint8_t* image, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t invert)
Draws a bitmap image at `x y` with `width` and `height`  
First argument expects 1D uint8_t array  
Your bitmap image has to be rotated 90° and flipped horizontaly

### void lcd_draw_character(uint8_t x, uint8_t y, font_table* font, char character)
Draws a single character at `x y` with specified font.  
There are 2 built-in fonts, `font_5x8` and `font_10x16`.  
Built-in fonts are limited to ASCII and does not include `~ $ @ |  ^ # &` characters _yet_.  
You can also make your custom fonts and use this function for them
.
### void lcd_draw_string(uint8_t x, uint8_t y, font_table* font, char string[])
Draws a series of characters with 1 pixel spacing between them at x, y

## Other

### void lcd_enable_pwm_brightness(uint8_t pin, uint8_t pwm_frequency)
Inits PWM brightness, frequency in `kHz`
Should work even if you underclocked/overclocked the MCU

### void lcd_set_brightness(uint8_t duty_cycle)
Sets the brightness level  
Duty cycle in % = brightness in %

### void lcd_toggle_invert()
Inverts all pixel values on the display, does NOT touch the framebuffer

### void lcd_shift_horizontaly(uint8_t shift_amount)
"Shifts" all pixels on the display, does NOT touch the framebuffer  
Pixels that are moved outside overflow to beggining

### void lcd_flip(uint8_t horizontaly, uint8_t verticaly)
Flips the display content horizontaly/verticaly, does NOT touch the framebuffer

### void lcd_shift_horizontaly(uint8_t shift_amount)
"Shifts" all pixels on the display, does NOT touch the framebuffer  
Pixels that are moved outside overflow to beggining

### void lcd_flip(uint8_t horizontaly, uint8_t verticaly)
Flips the display content horizontaly/verticaly, does NOT touch the framebuffer

### void lcd_shift_horizontaly(uint8_t shift_amount)
"Shifts" all pixels on the display, does NOT touch the framebuffer  
Pixels that are moved outside overflow to beggining

### void lcd_flip(uint8_t horizontaly, uint8_t verticaly)
Flips the display content horizontaly/verticaly, does NOT touch the framebuffer

### void lcd_shift_horizontaly(uint8_t shift_amount)
"Shifts" all pixels on the display, does NOT touch the framebuffer  
Pixels that are moved outside overflow to beggining

### void lcd_flip(uint8_t horizontaly, uint8_t verticaly)
Flips the display content horizontaly/verticaly, does NOT touch the framebuffer

### void lcd_shift_horizontaly(uint8_t shift_amount)
"Shifts" all pixels on the display, does NOT touch the framebuffer  

Pixels that are moved outside overflow to beggining

### void lcd_flip(uint8_t horizontaly, uint8_t verticaly)
Flips the display content horizontaly/verticaly, does NOT touch the framebuffer

