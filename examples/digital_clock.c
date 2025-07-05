// Shows driver being used in more "real world" scenario
// This example does NOT show current real world time

#include "../src/ST7567.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "pinout.h" // You can edit your pins here

uint16_t spi_frequency = 100; // kHz

char datetime_buf[256];
char *datetime_str = &datetime_buf[0];

datetime_t t = { 
  .year = 2025,
  .month = 4, 
  .day = 2, 
  .dotw = 3, // 0 is Sunday, so 5 is Friday
  .hour = 17, 
  .min = 54, 
  .sec = 39 
};

char clock_string[9];
char date_string[11];
char day_string[10];

int main(void) {
  // RTC
  rtc_init();
  rtc_set_datetime(&t);
  sleep_us(64);
  datetime_to_str(datetime_str, sizeof(datetime_buf), &t);

  // LCD
  lcd_spi_init(SPI_ID, MOSI_PIN, SCLK_PIN, DC_PIN, CS_PIN, RST_PIN, spi_frequency);
  lcd_init();
  lcd_clear_screen();
  lcd_set_contrast(4, 21);

  while (1) {
    rtc_get_datetime(&t);

    snprintf(clock_string, sizeof(clock_string), "%02d:%02d:%02d", t.hour, t.min, t.sec); // 17:54:39
    snprintf(date_string, sizeof(date_string), "%02d/%02d/%d", t.day, t.month, t.year); //  2/4/2025 (2. April)
    sscanf(datetime_buf, "%10s", day_string); // Wednesday
  
    lcd_clear_buffer();
    
    lcd_draw_string(1, 0, font_10x16, clock_string);
    lcd_draw_string(1, 20, font_10x16, date_string);
    lcd_draw_string(1, 40, font_10x16, day_string);

    lcd_display();
    sleep_ms(1000);
  }

  return 0;
}
