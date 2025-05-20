// Implementation of custom fonts
// Original font designs by Posy (Michiel de Boer) 2022 

#include "../../src/ST7567.h"
#include "../pinout.h" // You can edit your pins here

// The custom fonts
#include "seamless_segments.h"
#include "freeform_segments.h"

#define BLK_PIN 15
#define DIMMING_FREQUENCY 5 // kHz

uint16_t spi_frequency = 500; // kHz

int main() {

  lcd_spi_init(spi0, MOSI_PIN, SCLK_PIN, DC_PIN, CS_PIN, RST_PIN, spi_frequency);
  lcd_init();
  lcd_clear_screen();  

  lcd_enable_pwm_brightness(BLK_PIN, DIMMING_FREQUENCY);
  lcd_set_brightness(10);

  lcd_set_contrast(4, 21);


  char* num_list[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

  // Showcase of every number in these fonts
  for (uint8_t num = 0; num < 10; num++) {
    char* current_num = num_list[num];
    lcd_clear_buffer();

    lcd_draw_string(15, 0, font_5x8, "SEAMLESS");
    lcd_draw_string(27, 15, seamless_segments, current_num);
    lcd_draw_string(70, 0, font_5x8, "FREEFORM");
    lcd_draw_string(82, 15, freeform_segments, current_num);

    lcd_display();
    sleep_ms(900);
  }

  lcd_clear_buffer();
  sleep_ms(1000);
 
  // "Timer" from 9:99 0:00 to 0:00 9:99
  for (int8_t big_num = 9; big_num >= 0; big_num--) {
    for (int8_t small_num = 9; small_num >= 0; small_num--) {
      lcd_clear_buffer();

      lcd_draw_string(8, 2, font_5x8, "SEAMLESS");
      lcd_draw_string(76, 2, font_5x8, "FREEFORM");

      lcd_draw_string(5, 15, seamless_segments, num_list[big_num]);
      lcd_draw_string(35, 15, seamless_segments, num_list[small_num]);

      lcd_draw_string(73, 15, freeform_segments, num_list[9 - big_num]);
      lcd_draw_string(103, 15, freeform_segments, num_list[9 - small_num]);

      lcd_display();
      sleep_ms(200);
    }
  }
  
  // "Timer" from 0:00 9:99 to 9:99 0:00
  sleep_ms(500);
  lcd_toggle_invert();
  sleep_ms(500);

  for (int8_t big_num = 0; big_num < 10; big_num++) {
    for (int8_t small_num = 0; small_num < 10; small_num++) {
      lcd_clear_buffer();

      lcd_draw_string(8, 2, font_5x8, "SEAMLESS");
      lcd_draw_string(76, 2, font_5x8, "FREEFORM");

      lcd_draw_string(5, 15, seamless_segments, num_list[big_num]);
      lcd_draw_string(35, 15, seamless_segments, num_list[small_num]);

      lcd_draw_string(73, 15, freeform_segments, num_list[9 - big_num]);
      lcd_draw_string(103, 15, freeform_segments, num_list[9 - small_num]);

      lcd_display();
      sleep_ms(100);
    }
  }

  sleep_ms(1000);
  lcd_clear_buffer();
  lcd_toggle_invert();
  lcd_display();

  return 0;

}
