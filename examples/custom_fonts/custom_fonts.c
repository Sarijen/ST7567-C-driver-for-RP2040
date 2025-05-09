// Implementation of custom fonts
// Fonts by Posy (Michiel de Boer) 2022 

#include "../../src/ST7567.h"
#include "seamless_segments.h" // Our custom font
#include "x15_segments.h"

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
  lcd_set_brightness(10);

  lcd_set_contrast(4, 21);

  char* num_list[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};


  for (uint8_t num = 0; num < 10; num++) {
    char* current_num = num_list[num];
    lcd_draw_string(30, 10, seamless_segments, current_num);
    lcd_draw_string(79, 10, x15_segments, current_num);
    lcd_display();
    sleep_ms(900);
    lcd_clear_buffer();
  }

  return 0;

}
