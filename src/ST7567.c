#include "ST7567.h"

spiConfig spi = {
  0, // SPI instance
  0, // MOSI pin
  0, // SCLK pin
  0, // DC pin
  0, // CS pin
  0, // RST pin
};

uint8_t frameBuffer[NUM_PAGES * LCD_WIDTH]; // Contains 1024 Bytes, 128 Bytes per page

// Commands defined in binary for clear understanding
const uint8_t setPageAddr =   0b10110000;
const uint8_t setColAddrH =   0b00010000;
const uint8_t setColAddrL =   0b00000000; 
const uint8_t invertDisplay = 0b10100111;
int displayInverted = 0;
const uint8_t NOP =           0b11100011;
const uint8_t displayON =     0b10101111;
const uint8_t displayOFF =    0b10101110;
const uint8_t allPixelON =    0b10100100;
const uint8_t selectBias =    0b10100011; 
const uint8_t segDirection =  0b10100000; // | 0x01 for inverse
const uint8_t comDirection =  0b11000000; // | 0x08 for inverse
const uint8_t EVmode =        0b10000001;
const uint8_t EVset =         0b00000000; // Contrast control 
const uint8_t RegRatio =      0b00100000;
const uint8_t powerControl =  0b00101111;
const uint8_t setStartLine =  0b01000000; 

const uint8_t blankData =     0b00000000;

void lcd_draw_image(uint8_t* image, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  uint16_t currentByte = 0;
  for (int8_t i = x; i < x + width; i++) { // Individual columns
    uint16_t currentY = y;
    for (int8_t j = 0; j < (height / 8); j++) {  // Individual bytes in 1 col
      for (int8_t bit = 7; bit >= 0; bit--) {  // Individual bits in 1 byte
        // If the bit is NOT 1, draw pixel (inverted)
        if (!((image[currentByte] >> bit) & 1)) {
          lcd_draw_pixel(i, currentY, 1); 
        }
        currentY++;
      }
      currentByte++;
    }
  }
}

void lcd_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t value) {
  uint8_t dx = x + width;
  uint8_t dy = y + height;

  for (uint8_t i = x; i < dx; i++) { // We supplement x, y with i, j so they reset after looping
    for (uint8_t j = y; j < dy; j++) {
      lcd_draw_pixel(i, j, value);
    }            //  x  y
  }
}

void lcd_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t value) {
  uint8_t dx = x + width; 
  uint8_t dy = y + height;

  for (uint8_t i = x; i < dx; i++) {
    for (uint8_t j = y; j < dy; j++) {
      if (i == x || i == dx - 1 || j == y || j == dy - 1) { // Only draw pixels on outer layers
        lcd_draw_pixel(i, j, value); 
      }
    }
  }
}

void lcd_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value) { // Bresenham's algorithm used
  uint8_t dx = abs(x2 - x1); 
  uint8_t dy = abs(y2 - y1);
  int8_t sx = (x1 < x2) ? 1 : -1;
  int8_t sy = (y1 < y2) ? 1 : -1;
  int16_t error = dx - dy;

  while (x1 != x2 || y1 != y2) {
    lcd_draw_pixel(x1, y1, value);

    int16_t e2 = 2*error;
    if (e2 > -dy) {
      error -= dy;
      x1 += sx;
    }
    if (e2< dx) {
      error += dx;
      y1 += sy;
    }
  }
}

void lcd_draw_pixel(uint8_t x, uint8_t y, uint8_t value) { // Calculates x, y to bits location in the buffer
  uint8_t page = y / 8;
  uint16_t byte_index = (128 * page + x);
  uint8_t bit_position = (y % 8);
  if (value == 0) {
    frameBuffer[byte_index] &= ~(1 << bit_position);  
  } else {
    frameBuffer[byte_index] |= (1 << bit_position);
  }
  //lcd_display(); // Sends the entire buffer after every pixel draw, VERY SLOW!
}

void lcd_toggle_invert() {
  if (displayInverted) {
    send_command(invertDisplay & ~1); // Flips the last bit
    displayInverted = 0;
  } else {
    send_command(invertDisplay);
    displayInverted = 1;
  }
}

void lcd_display() {
  for (uint8_t currentPage = 0; currentPage < 8; currentPage++) { // Set the right Page and Col addresses
    send_command(setPageAddr | currentPage);
    send_command(setColAddrH); // Resets the Column Address for every page
    send_command(setColAddrL);

    for (uint16_t j = currentPage*128; j < (currentPage*128) + 128; j++) { // Every page is 128 bytes long 
      send_data(frameBuffer[j]); // Sending bytes of buffer to the display
    }
  }
}

void lcd_clear_buffer() { // Fills the buffer on MCU with 0s
  for (uint16_t byte = 0; byte < 1024; byte++) {
    frameBuffer[byte] = blankData;
  }
}

void lcd_clear_screen() { // Fills the screen with 0s
  for (uint8_t currentPage = 0; currentPage < 8; currentPage++) {
    send_command(setPageAddr | currentPage);
    send_command(setColAddrH); 
    send_command(setColAddrL); 

    for (uint8_t j = 0; j < 128; j++) { 
      send_data(blankData);
    }
  }
}

pwmConfig pwm = {
  69, // slice num (-1 if doesn't exist)
  0,  // wrapping point
  15  // pin number
};

void lcd_enable_pwm_brightness(uint8_t pin, uint8_t frequency) {
  pwm.pin = pin;
  pwm.wrapping_point = 1000000 / (frequency * 8);  
  
  gpio_set_function(pwm.pin, GPIO_FUNC_PWM);
  pwm.slice_num = pwm_gpio_to_slice_num(pwm.pin);
  pwm_set_wrap(pwm.slice_num, pwm.wrapping_point);
}

void lcd_set_brightness(uint8_t duty_cycle) {
  if (pwm.slice_num == 69) {
    printf("Error: You need to call lcd_enable_pwm_brigthness() first!\n");
    return;
  }
  if (duty_cycle > 100) {
    duty_cycle = 100;
    printf("Warning: Brightness cannot exceed 100%, defaulted back to 100%.\n");
    return;
  }
  if (duty_cycle == 0) {
    pwm_set_enabled(pwm.slice_num, false);
    return;
  }

  // Duty cycle / 100 converts % to decimal, so we can multiply it with wrapping point
  // We multiply duty cycle first and then convert, thus avoiding floating-point operations
  uint16_t time_ON = (duty_cycle * pwm.wrapping_point) / 100;

  if ((pwm.pin % 2) == 0) { // Even pins are channel A, B are odd
    pwm_set_chan_level(pwm.slice_num, PWM_CHAN_A, time_ON);
  } else {
    pwm_set_chan_level(pwm.slice_num, PWM_CHAN_B, time_ON);
  }
  pwm_set_enabled(pwm.slice_num, true);
}

void lcd_set_contrast(uint8_t value) {
  if (value > 63) {
    printf("Error: Value %d is outside the allowed range for contrast", value);
    return;
  }
  send_command(EVmode);
  send_command(EVset | value);
}

inline void send_command(uint8_t command) {
  gpio_put(spi.CS, 0);
  spi_write_blocking(spi.ID, &command, 1);
  gpio_put(spi.CS, 1);
}

inline void send_data(uint8_t data) {
  gpio_put(spi.DC, 1);
  gpio_put(spi.CS, 0);
  spi_write_blocking(spi.ID, &data, 1);
  gpio_put(spi.CS, 1);
  gpio_put(spi.DC, 0);
}

void lcd_reset() {
  gpio_put(spi.RST, 0);
  sleep_us(2);
  gpio_put(spi.RST, 1);
  sleep_us(2);
}

void lcd_init() {
  lcd_reset();

  send_command(displayOFF);
  send_command(selectBias);
  send_command(segDirection);
  send_command(comDirection | 0x08);
  send_command(powerControl);
  sleep_us(50);

  send_command(RegRatio | 0b100);
  send_command(EVmode);
  send_command(EVset); // 0x1F recommended for optimal contrast

  send_command(setStartLine);
  send_command(displayON);
  send_command(allPixelON);
}

void lcd_spi_init(
    spi_inst_t* spi_id, 
    uint8_t MOSI,
    uint8_t SCLK,
    uint8_t DC,
    uint8_t CS,
    uint8_t RST,
    uint16_t frequency) {

  if (frequency > 20000) {
    printf("Warning: %d kHz SCLK is higher than 20MHz stated in the datasheet at 3.3V, 25°C.\n", frequency);
  }

  spi.ID = spi_id;
  spi.MOSI = MOSI;
  spi.SCLK = SCLK;
  spi.DC = DC;
  spi.CS = CS;
  spi.RST = RST;

  gpio_init(spi.DC);
  gpio_init(spi.CS);
  gpio_init(spi.RST);

  gpio_set_dir(spi.DC, GPIO_OUT);
  gpio_set_dir(spi.CS, GPIO_OUT);
  gpio_set_dir(spi.RST, GPIO_OUT);

  gpio_put(spi.CS, 1); 
  gpio_put(spi.DC, 0);
  gpio_put(spi.RST, 1);

  spi_init(spi.ID, 1000 * frequency);
  gpio_set_function(spi.MOSI, GPIO_FUNC_SPI);
  gpio_set_function(spi.SCLK, GPIO_FUNC_SPI);
  spi_set_format(spi.ID,
    8,             // 8 bits per transfer
    SPI_CPOL_0,    // Clock polarity 
    SPI_CPHA_0,    // Clock phase 
    SPI_MSB_FIRST  // Most significant bit first
  );
}
