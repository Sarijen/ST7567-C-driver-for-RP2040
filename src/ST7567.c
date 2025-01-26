#include "ST7567.h"

uint8_t frameBuffer[1024]; // Contains 128x bytes, 16 bytes for each Page

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
const uint8_t EVset =         0b00000000;
const uint8_t REGratio =      0b00100010;
const uint8_t powerControl =  0b00101111;
const uint8_t setStartLine =  0b01000000; 

const uint8_t blankData =     0b00000000;

void lcd_draw_image(uint8_t* image, int x, int y, int width, int height) {
  int currentByte = 0;
  for (int i = x; i < x + width; i++) { // Individual columns
    int currentY = y;
    for (int j = 0; j < (height / 8); j++) {  // Individual bytes in 1 col
      for (int bit = 7; bit >= 0; bit--) {  // Individual bits in 1 byte
        // If the bit is NOT 1, draw pixel (inverted)
        if (!((image[currentByte] >> bit) & 1)) {
          lcd_draw_pixel(i, currentY); 
        }
        currentY++;
      }
      currentByte++;
    }
  }
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
  int error = dx - dy;

  while (x1 != x2 || y1 != y2) {
    lcd_draw_pixel(x1, y1);

    int e2 = 2*error;
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


void lcd_draw_pixel(int x, int y) { // Calculates x, y to bits location in the buffer
  int page = y / 8;
  int byte_index = (128 * page + x);
  int bit_offset = (y % 8);
  frameBuffer[byte_index] |= (1 << bit_offset);
  //  lcd_display(); // Sends the entire buffer after every pixel draw, VERY SLOW!
}

void lcd_invert() {
  if (displayInverted) {
    send_command(invertDisplay & ~1); // Flips the last bit
    displayInverted = 0;
  } else {
    send_command(invertDisplay);
    displayInverted = 1;
  }
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

void lcd_clear_buffer() { // Fills the buffer on MC with 0s
  for (int byte = 0; byte < 1024; byte++) {
    frameBuffer[byte] = blankData;
  }
}

void lcd_clear_screen() { // Fills the screen with 0s
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
  gpio_put(pin.CS, 0);
  spi_write_blocking(spi0, &command, 1);
  gpio_put(pin.CS, 1);
}

inline void send_data(uint8_t data) {
  gpio_put(pin.DC, 1);
  gpio_put(pin.CS, 0);
  spi_write_blocking(spi0, &data, 1);
  gpio_put(pin.CS, 1);
  gpio_put(pin.DC, 0);
}

void lcd_reset() {
  gpio_put(pin.RST, 0);
  sleep_us(3);
  gpio_put(pin.RST, 1);
  sleep_us(3);
}

void lcd_init() {
  lcd_reset();

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

void lcd_spi_init(spi_inst_t* spi, uint frequency) {
  gpio_init(pin.DC);
  gpio_init(pin.CS);
  gpio_init(pin.RST);

  gpio_set_dir(pin.DC, GPIO_OUT);
  gpio_set_dir(pin.CS, GPIO_OUT);
  gpio_set_dir(pin.RST, GPIO_OUT);

  gpio_put(pin.CS, 1); 
  gpio_put(pin.DC, 0);
  gpio_put(pin.RST, 1);

  spi_init(spi, 1000 * frequency);
  gpio_set_function(pin.MOSI, GPIO_FUNC_SPI);
  gpio_set_function(pin.SCLK, GPIO_FUNC_SPI);
  spi_set_format(spi,
    8,             // 8 bits per transfer
    SPI_CPOL_0,    // Clock polarity 
    SPI_CPHA_0,    // Clock phase 
    SPI_MSB_FIRST  // Most significant bit first
  );
}
