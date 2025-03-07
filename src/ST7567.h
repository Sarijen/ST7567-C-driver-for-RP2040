#ifndef ST7567_H
#define ST7567_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include <stdlib.h>

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define NUM_PAGES 8

typedef struct {
  spi_inst_t* ID; // SPI instance (spi0/spi1)
  uint8_t MOSI;
  uint8_t SCLK;
  uint8_t DC;
  uint8_t CS;
  uint8_t RST;
} spiConfig;

typedef struct {
  uint8_t slice_num;
  uint16_t wrapping_point;
  uint8_t pin;
} pwmConfig;

void send_command(uint8_t command);
void send_data(uint8_t data);
void lcd_spi_init(
    spi_inst_t* spi_id,
    uint8_t MOSI,
    uint8_t SCLK,
    uint8_t DC,
    uint8_t CS,
    uint8_t RST,
    uint16_t frequency); // in kHz

void lcd_init();
void lcd_reset();
void lcd_set_contrast(uint8_t value);
void lcd_enable_pwm_brightness(uint8_t pin, uint8_t frequency); // frequency in kHz
void lcd_set_brightness(uint8_t duty_cycle); // in %
void lcd_clear_screen();
void lcd_clear_buffer();
void lcd_display();
void lcd_toggle_invert();
void lcd_draw_pixel(uint8_t x, uint8_t y, uint8_t value);
void lcd_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value);
void lcd_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t value);
void lcd_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t value);
void lcd_draw_image(uint8_t* image, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

#endif 
