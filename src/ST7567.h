#ifndef ST7567_H
#define ST7567_H

#include "fonts.h"

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  spi_inst_t* ID;  // SPI instance (spi0/spi1)
  uint8_t MOSI;
  uint8_t SCLK;
  uint8_t DC;      // A0 in the datasheet
  uint8_t CS;
  uint8_t RST;
} spi_config_t;

typedef struct {
  uint8_t slice_num;
  uint16_t wrapping_point;
  uint8_t pin;
} pwm_config_t;

static inline void send_command(uint8_t command);
static inline void send_data(uint8_t data);
void lcd_spi_init(
    spi_inst_t* spi_id,
    uint8_t MOSI,
    uint8_t SCLK,
    uint8_t DC,
    uint8_t CS,
    uint8_t RST,
    uint16_t spi_frequency); // in kHz

void lcd_init(void);
void lcd_reset(void);
void lcd_set_contrast(uint8_t RR_value, uint8_t EV_value);
void lcd_enable_pwm_brightness(uint8_t pin, uint8_t pwm_frequency); // frequency in kHz
void lcd_set_brightness(uint8_t duty_cycle); // in %

void lcd_clear_screen(void);
void lcd_clear_buffer(void);
void lcd_display(void);
void lcd_toggle_invert(void);
void lcd_shift_horizontally(uint8_t shift_amount);
void lcd_flip(uint8_t horizontally, uint8_t vertically);

void lcd_draw_pixel(uint8_t x, uint8_t y, uint8_t value);
void lcd_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value);
void lcd_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t value);
void lcd_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t value);
void lcd_draw_image(uint8_t* image, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t invert);
void lcd_draw_character(uint8_t x, uint8_t y, font_table* font, char character);
void lcd_draw_string(uint8_t x, uint8_t y, font_table* font, char string[]);

#endif 
