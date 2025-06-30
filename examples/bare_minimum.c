// Bare minimum code to test if your hardware and wiring are correct
// Smiley face not required

#include "../src/ST7567.h"
#include "pinout.h" // You can edit your pins here

uint16_t spi_frequency = 100; // kHz

int main() {
  lcd_spi_init(SPI_ID, MOSI_PIN, SCLK_PIN, DC_PIN, CS_PIN, RST_PIN, spi_frequency);
  lcd_init();
  lcd_clear_screen();
  lcd_set_contrast(4, 21);

  // Smiley face :]
  lcd_fill_rect(35, 10, 10, 10, 1); // Left eye
  lcd_fill_rect(75, 10, 10, 10, 1); // Right eye

  // Mouth
  lcd_draw_line(30, 30, 30, 40, 1); 
  lcd_draw_line(30, 40, 90, 40, 1);
  lcd_draw_line(90, 30, 90, 40, 1);

  lcd_display(); 

  // Cycle between various contrast values if the recommended isn't visible
  sleep_ms(3500);
  lcd_set_contrast(1, 10);
  sleep_ms(3500);
  lcd_set_contrast(2, 10);
  sleep_ms(3500);
  lcd_set_contrast(5, 10);
  sleep_ms(3500);
  lcd_set_contrast(6, 10);


  return 0;
}
