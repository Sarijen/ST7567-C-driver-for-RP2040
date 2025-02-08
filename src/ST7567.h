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
  const int MOSI;
  const int SCLK;
  const int DC;
  const int CS;
  const int RST;
} pinConfig;

extern pinConfig pin;

typedef struct {
  int slice_num;
  int wrapping_point;
  int pin;
} pwmConfig;

void send_command(uint8_t command);
void send_data(uint8_t data);
void lcd_spi_init(spi_inst_t*, uint frequency); // in kHz
void lcd_init();
void lcd_reset();
void lcd_enable_pwm_brigthness(int pin, int frequency); // frequency in kHz
void lcd_set_brigthness(int duty_cycle); // in %
void lcd_clear_screen();
void lcd_clear_buffer();
void lcd_display();
void lcd_toggle_invert();
void lcd_draw_pixel(int x, int y);
void lcd_draw_line(int x1, int y1, int x2, int y2);
void lcd_draw_rect(int x, int y, int width, int height);
void lcd_fill_rect(int x, int y, int width, int height);
void lcd_draw_image(uint8_t* image, int x, int y, int width, int height);

#endif 
