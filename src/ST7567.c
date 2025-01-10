#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <stdlib.h>

#define MOSI_PIN 3 // SDI
#define SCLK_PIN 2 // SCK
#define DC_PIN 5 // A0 in the datasheet
#define CS_PIN 6
#define RST_PIN 7 // Hardware Reset

#define LCD_WIDTH 128
#define LCD_HEIGHT 64

uint8_t setPageAddr =   0b10110000;
uint8_t setColAddrH =   0b00010000;
uint8_t setColAddrL =   0b00000000; 
uint8_t invertDisplay = 0b10100111;
uint8_t NOP =           0b11100011;
uint8_t blankData =     0b00000000;

uint8_t frameBuffer[1024]; // Contains 128x bytes, 16 bytes for each Page

void send_command(uint8_t command);
void send_data(uint8_t data);
void lcd_spi_init(uint frequency); // in kHz
void lcd_init();
void lcd_clear();
void lcd_display();
void lcd_draw_pixel(int x, int y);
void lcd_draw_line(int x1, int y1, int x2, int y2);
void lcd_draw_rect(int x, int y, int width, int height);
void lcd_fill_rect(int x, int y, int width, int height);

int main() {
  stdio_init_all();  
  printf("Init!\n");

  lcd_spi_init(200); 
  lcd_init();
  lcd_clear();

  lcd_draw_line(0, 0, 128, 64);
  lcd_draw_line(0, 64, 128, 0);
 
  //8*16 Checkerboard pattern
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if ((j % 2) == 0) {
        lcd_draw_rect(i*16, j*8, 8, 8);
      } else {
        lcd_draw_rect((i*16) + 8, j*8, 8, 8);
      }
      sleep_ms(50);
      lcd_display();
    }
  }

  lcd_display();
  return 0;
}


void lcd_fill_rect(int x, int y, int width, int height) {
  int dx = x + width;
  int dy = y + height;

  for (int i = x; i < dx; i++) { // We supplement x, y with i, j so they reset after looping
    for (int j = y; j < dy; j++) {
      lcd_draw_pixel(i, j);
    }            //  x  y
  }
}

void lcd_draw_rect(int x, int y, int width, int height) {
  int dx = x + width;
  int dy = y + height;

  for (int i = x; i < dx; i++) {
    for (int j = y; j < dy; j++) {
      if (i == x || i == dx - 1 || j == y || j == dy - 1) { // Only draw pixels on outer layers
        lcd_draw_pixel(i, j); 
      }
    }
  }
}

void lcd_draw_line(int x1, int y1, int x2, int y2) { // Bresenham's algorithm used
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;

  while (x1 != x2 && y1 != y2) {
    lcd_draw_pixel(x1, y1); 

    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
}


void lcd_draw_pixel(int x, int y) { // Calculates x, y to bits location in the buffer
  int page = y / 8;
  int byte_index = (128 * page + x);
  int bit_offset = (y % 8);
  frameBuffer[byte_index] |= (1 << bit_offset);
//  lcd_display(); // Sends the entire buffer after every pixel draw, VERY SLOW!
}

void lcd_display() {
  for (int currentPage = 0; currentPage < 8; currentPage++) { // Set the right Page and Col addresses
    send_command(setPageAddr | currentPage);
    send_command(setColAddrH); // Resets the Column Address for every page
    send_command(setColAddrL);

    for (int j = currentPage*128; j < (currentPage*128) + 128; j++) { // Every page is 128 bytes long 
      send_data(frameBuffer[j]); // Sending bytes of buffer to the display
    }
  }
}

void lcd_clear() { // Writes 0s to the display, not touching framebuffer
  for (int currentPage = 0; currentPage < 8; currentPage++) {
    send_command(setPageAddr | currentPage);
    send_command(setColAddrH); 
    send_command(setColAddrL); 

    for (int j = 0; j < 128; j++) { 
      send_data(blankData);
    }
  }
}

inline void send_command(uint8_t command) {
  gpio_put(CS_PIN, 0);
  spi_write_blocking(spi0, &command, 1);
  gpio_put(CS_PIN, 1);
}

inline void send_data(uint8_t data) {
  gpio_put(DC_PIN, 1);
  gpio_put(CS_PIN, 0);
  spi_write_blocking(spi0, &data, 1);
  gpio_put(CS_PIN, 1);
  gpio_put(DC_PIN, 0);
}


void lcd_init() {
  gpio_put(RST_PIN, 0);
  sleep_us(6);
  gpio_put(RST_PIN, 1);
  sleep_us(6);

  uint8_t displayON =     0b10101111;
  uint8_t displayOFF =    0b10101110;
  uint8_t allPixelON =    0b10100100;
  uint8_t selectBias =    0b10100011; 
  uint8_t segDirection =  0b10100000; // | 0x01 for inverse
  uint8_t comDirection =  0b11000000; // | 0x08 for inverse
  uint8_t EVmode =        0b10000001;
  uint8_t EVset =         0b00000000;
  uint8_t REGratio =      0b00100010;
  uint8_t powerControl =  0b00101111;
  uint8_t setStartLine =  0b01000000; 

  send_command(displayOFF);
  send_command(selectBias);
  send_command(segDirection);
  send_command(comDirection | 0x08);
  send_command(powerControl);
  sleep_us(50);

  send_command(REGratio); // You might need to change Regulation Ratio for your specific display
  send_command(EVmode);
  send_command(EVset);

  send_command(setStartLine);
  send_command(displayON);
  send_command(allPixelON);
}

void lcd_spi_init(uint frequency) {
  gpio_init(CS_PIN);
  gpio_init(DC_PIN);
  gpio_init(RST_PIN);

  gpio_set_dir(CS_PIN, GPIO_OUT);
  gpio_set_dir(RST_PIN, GPIO_OUT);
  gpio_set_dir(DC_PIN, GPIO_OUT);

  gpio_put(CS_PIN, 1); 
  gpio_put(DC_PIN, 0);

  spi_init(spi0, 1000 * frequency);
  gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
  gpio_set_function(SCLK_PIN, GPIO_FUNC_SPI);
  spi_set_format(spi_default,
    8,             // 8 bits per transfer
    SPI_CPOL_0,    // Clock polarity 
    SPI_CPHA_0,    // Clock phase 
    SPI_MSB_FIRST  // Most significant bit first
  );
}
