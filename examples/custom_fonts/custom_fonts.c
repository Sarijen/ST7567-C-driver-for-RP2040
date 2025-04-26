// Implementation of custom fonts
// Font created by Posy (Michiel de Boer) 2022 

#include "../../src/ST7567.h"
#include "seamless_segments.h" // Our custom font

// CHANGE THESE!
#define MOSI_PIN 3 
#define SCLK_PIN 2 
#define DC_PIN 5
#define CS_PIN 6
#define RST_PIN 7

#define SPI_ID spi0

#define BLK_PIN 15
#define DIMMING_FREQUENCY 5 // kHz

uint16_t spi_frequency = 100; // kHz

int main() {

  lcd_spi_init(spi0, MOSI_PIN, SCLK_PIN, DC_PIN, CS_PIN, RST_PIN, spi_frequency);
  lcd_init();
  lcd_clear_screen();  

  lcd_enable_pwm_brightness(BLK_PIN, DIMMING_FREQUENCY);
  lcd_set_brightness(50);

  lcd_set_contrast(4, 21);

  //              x   y   custom font name   string
  lcd_draw_string(1,  10, seamless_segments, "17");
  lcd_draw_string(70, 10, seamless_segments, "54");

  uint8_t dot_size = 8;
  while (1) {
    lcd_fill_rect(55, 15, dot_size, dot_size, 1);
    lcd_fill_rect(55, 35, dot_size, dot_size, 1);
    lcd_display();

    sleep_ms(1000);

    lcd_fill_rect(55, 15, dot_size, dot_size, 0);
    lcd_fill_rect(55, 35, dot_size, dot_size, 0);
    lcd_display();

    sleep_ms(1000);  
  }

  return 0;

}
