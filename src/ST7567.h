#ifndef ST7567_H
#define ST7567_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <stdlib.h>

/*
#define MOSI_PIN 3 // SDI
#define SCLK_PIN 2 // SCK
#define DC_PIN 5 // A0 in the datasheet
#define CS_PIN 6
#define RST_PIN 7 // Hardware Reset
*/
#define LCD_WIDTH 128
#define LCD_HEIGHT 64

typedef struct {
  const int MOSI;
  const int SCLK;
  const int DC;
  const int CS;
  const int RST;
} pinConfig;

extern pinConfig pin;

void send_command(uint8_t command);
void send_data(uint8_t data);
void lcd_spi_init(spi_inst_t*, uint frequency); // in kHz
void lcd_init();
void lcd_clear();
void lcd_display();
void lcd_draw_pixel(int x, int y);
void lcd_draw_line(int x1, int y1, int x2, int y2);
void lcd_draw_rect(int x, int y, int width, int height);
void lcd_fill_rect(int x, int y, int width, int height);

#endif 
