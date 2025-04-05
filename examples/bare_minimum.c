// Bare minimum code to test if your hardware and wiring are correct
// Smiley face not required

#include "../src/ST7567.h"

// CHANGE THESE
#define MOSI_PIN 3
#define SCLK_PIN 2
#define DC_PIN 5
#define CS_PIN 6
#define RST_PIN 7

#define SPI_ID spi0

uint16_t spi_frequency = 100; // kHz

int main() {
  lcd_spi_init(SPI_ID, MOSI_PIN, SCLK_PIN, DC_PIN, CS_PIN, RST_PIN, spi_frequency);
  lcd_init();
  lcd_clear_screen();

  // Smiley face :]
  lcd_fill_rect(35, 10, 10, 10, 1); // Left eye
  lcd_fill_rect(75, 10, 10, 10, 1); // Right eye

  // Mouth
  lcd_draw_line(30, 30, 30, 40, 1); 
  lcd_draw_line(30, 40, 90, 40, 1);
  lcd_draw_line(90, 30, 90, 40, 1);

  lcd_display(); 

  return 0;
}
