// Code used to showcase features of this driver library

#include "../src/ST7567.h"
#include "pinout.h" // You can edit your pins here
#include "showcase_image.c"

#define BLK_PIN 15
#define DIMMING_FREQUENCY 5 // kHz

uint16_t spi_frequency = 700; // kHz

void transition();

int main() {
// ==================== INIT ==============================

  lcd_spi_init(spi0, MOSI_PIN, SCLK_PIN, DC_PIN, CS_PIN, RST_PIN, spi_frequency);
  lcd_init();
  lcd_clear_screen();

  lcd_enable_pwm_brightness(BLK_PIN, DIMMING_FREQUENCY);
  lcd_set_brightness(50); // %

  // First argument changes contrast a lot, second is for further fine tuning
  lcd_set_contrast(4, 21);
 
  while(1) {
// ==================== INTRO ==============================

    //              x   y   font type string
    lcd_draw_string(11, 11, font_5x8, "ST7567 LCD Display");
    lcd_draw_string(11, 21, font_5x8, "Running on RP2040");
    lcd_draw_string(11, 31, font_5x8, "Using Pico C SDK");
    lcd_draw_string(11, 41, font_5x8, "Made by Sarijen");

    //            x1    y1  x2    y2 BLACK/WHITE
    lcd_draw_line(8,    8,  8,    50, 1);
    lcd_draw_line(8,    8,  120,  8,  1);
    lcd_draw_line(120,  8,  120,  50, 1);
    lcd_draw_line(8,    50, 120,  50, 1);

    lcd_display();
    transition();


// ==================== ANIMATION TRANSITION ==============================

  for (int i = 127; i > 0; i--) {
    lcd_draw_line(i, 0, 127,   64, 1);
    lcd_draw_line(0, 0, 127-i, 64, 1);
    lcd_display();
  }
  lcd_clear_buffer();


// ==================== SHAPES AND PATTERNS ==============================

    // == Top-Left corner thing ==
    for (int i = 0; i < 9; i++) {
      lcd_draw_line(0, 0, 48-6*i, 4*i, 1);
    }

    //            x   y   w   h  BLACK/WHITE
    lcd_draw_rect(20, 20, 21, 21, 1);  

    //             x   y  BLACK/WHITE
    lcd_draw_pixel(25, 25, 1);
    lcd_draw_pixel(25, 35, 1);
    lcd_draw_pixel(35, 25, 1);
    lcd_draw_pixel(35, 35, 1);

    // == ZIG-ZAG pattern ==
    int y = 63;
    int wave = -1;
    for (int i = 0; i < 127; i++) {
      lcd_draw_pixel(i, y, 1);
      y += wave;
      if (y == 63) {wave = -1;}
      if (y == 56) {wave = 1;}
    }
 
    // == Checkerboard pattern ==
    for (int i = 0; i < 128; i++) {
      lcd_draw_pixel(i, 52 + i % 2, 1);
    }

    // == Triangle ==
    lcd_draw_line(45, 20, 60, 25, 1); 
    lcd_draw_line(45, 20, 50, 35, 1);
    lcd_draw_line(50, 35, 60, 25, 1);

    // Random boxes in Top Right Corner
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        lcd_draw_pixel(90+8*i, 1+8*j, 1);
        lcd_draw_pixel(90+8*i, 3+8*j, 1);
        lcd_draw_pixel(92+8*i, 1+8*j, 1);
        lcd_draw_pixel(92+8*i, 3+8*j, 1);
      }
    }

    lcd_draw_string(10, 42, font_5x8, "Shapes and patterns");

    lcd_display();
    transition();


// ==================== ANIMATION TRANSITION ==============================

  for (int i = 0; i < 64; i++) {
    lcd_draw_line(0, 63-i, 128, 63, 1);
    lcd_draw_line(0, 0,    128, i,  1);
    lcd_display();
    sleep_ms(2);
  }
  lcd_clear_buffer();


// ==================== IMAGES ==============================

    lcd_draw_string(20, 1, font_5x8, "Bitmap images");

//                 image array     x   y   w   h  invert (True/False)
    lcd_draw_image(showcase_image, 5,  10, 48, 56, 0);
    lcd_draw_image(showcase_image, 71, 10, 48, 56, 1);

    lcd_display();
    transition();


// ==================== FONTS ==============================
// Only font_5x8, font_10x16 and custom fonts are available

    //              x   y  font struct  string
    lcd_draw_string(25, 1, font_5x8,    "Bitmap fonts");
    lcd_draw_line(0, 10, 127, 10, 1);
    lcd_draw_string(1, 15, font_5x8,    "ABCDEFGHIJKLMNOPQRSTU");
    lcd_draw_string(1, 25, font_5x8,    "VWXYZabcdefghijklmnop");
    lcd_draw_string(1, 35, font_5x8,    "qrstuvwxyz0123456789|");
    lcd_draw_string(1, 45, font_5x8,    "()[]<>!?,.;:*+-=/_%@~");
    lcd_draw_string(1, 55, font_5x8,    "&#      17. May, 2025");

    lcd_display();
    transition();

    lcd_draw_string(2, 0, font_10x16,  "ABCDEFGHIJK");
    lcd_draw_string(2, 16, font_10x16, "LMNOPQRSTUV");
    lcd_draw_string(2, 32, font_10x16, "WXYZabcdefg");
    lcd_draw_string(2, 48, font_10x16, "ijklmnopqrs");

    lcd_display();
    sleep_ms(2000);
    lcd_clear_buffer();

    lcd_draw_string(2, 0, font_10x16,  "tuvwxyz0123");
    lcd_draw_string(2, 16, font_10x16, "456789|()[]");
    lcd_draw_string(2, 32, font_10x16, "<>!?,.:*+-=");
    lcd_draw_string(2, 48, font_10x16, "_%/@~&#");
   
    lcd_display();
    transition();


// ==================== PWM BRIGHTNESS ==============================

    lcd_draw_string(3, 1, font_10x16, "PWM");
    lcd_draw_string(3, 20, font_10x16, "Brightness");
    lcd_draw_string(3, 40, font_10x16, "Control");
    lcd_display();
    sleep_ms(1000);

    for (uint8_t percent = 100; percent > 0; percent--) {
      lcd_set_brightness(percent);
      sleep_ms(20);
    }

    lcd_set_brightness(0);
    sleep_ms(1000);
    lcd_set_brightness(30);
    sleep_ms(1000);
    lcd_set_brightness(60);
    sleep_ms(1000);
    lcd_set_brightness(90);
    sleep_ms(1000);

    lcd_set_brightness(50);
    lcd_clear_buffer();
  }
}

void transition() {
  sleep_ms(3500);
  lcd_toggle_invert();
  sleep_ms(1000); 
  lcd_toggle_invert();
  lcd_clear_buffer();
}
