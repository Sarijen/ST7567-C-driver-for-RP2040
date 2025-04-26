#include "ST7567.h"

static spiConfig spi;

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define PAGE_COUNT 8

uint8_t frameBuffer[PAGE_COUNT * LCD_WIDTH]; // Contains 1024 Bytes, 128 Bytes per page

// Commands defined in binary for clear understanding
// # CONFIGURATION
static const uint8_t EVmode =           0b10000001;
static const uint8_t EVset =            0b00000000;
static const uint8_t regRatio =         0b00100000;
static const uint8_t selectBias =       0b10100011;
static const uint8_t powerControl =     0b00101111;

static const uint8_t displayOFF =       0b10101110;
static const uint8_t displayON =        displayOFF | 0x01;

// # PIXEL DATA MANIPULATION
static const uint8_t setStartLine =     0b01000000;
static const uint8_t allPixelsNormal =  0b10100100;
static const uint8_t allPixelsON =      allPixelsNormal | 0x01;

static const uint8_t setPageAddr =      0b10110000;
static const uint8_t setColAddrH =      0b00010000;
static const uint8_t setColAddrL =      0b00000000;

static const uint8_t segDirection =     0b10100000;
static const uint8_t segInvDirection =  segDirection | 0x01;
static const uint8_t comDirection =     0b11000000; 
static const uint8_t comInvDirection =  comDirection | 0x08; 

static const uint8_t invertDisplay =    0b10100111;
static uint8_t displayInverted = 0;

// OTHER
static const uint8_t NOP =              0b11100011;
static const uint8_t reset =            0b11100010;

static const uint8_t blankData = 0x00;

void lcd_draw_string(uint8_t x, uint8_t y, font_table* font, char string[]) {
  uint8_t string_length = strlen(string);
  for (uint8_t i = 0; i < string_length; i++) {
    lcd_draw_character(x + ((font[0].width + 1) * i), y, font, string[i]);
  }
}

void lcd_draw_character(uint8_t x, uint8_t y, font_table* font, char character) {
  uint8_t* bitmap_data;
  uint8_t matching_char = 0;

  while (1) { // Find our character in the font table
    if (character == font[matching_char].character) {
      bitmap_data = font[matching_char].bitmap_data;
      break;
    }

    // If a character that isn't supported is given, use the first character
    // End of font table is indicated as a character with width = 0
    if (font[matching_char].width == 0) {
      bitmap_data = font[0].bitmap_data;
      break;
    }

    matching_char++;
  }

  // Draw our character
  uint8_t currentByte = 0;
  for (int i = x; i < x + font[matching_char].width; i++) {
    int currentY = y;
    for (int j = 0; j < (font[matching_char].height / 8); j++) {
      for (int bit = 0; bit < 8; bit++) {
        if (!((bitmap_data[currentByte] >> bit) & 1)) {
          lcd_draw_pixel(i, currentY, 1);
        }
        currentY++;
      }
      currentByte++;
    }
  }
  
}

void lcd_draw_image(uint8_t* image, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t invert) {
  if (invert > 1) {invert = 1;}

  uint16_t currentByte = 0;
  for (int8_t i = x; i < x + width; i++) { // Individual columns
    uint16_t currentY = y;

    for (int8_t j = 0; j < (height / 8); j++) {  // Individual bytes in 1 col

      for (int8_t bit = 7; bit >= 0; bit--) {  // Individual bits in 1 byte

        if (((image[currentByte] >> bit) & 1) == invert) {
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
  uint8_t page = y / PAGE_COUNT;
  uint16_t byte_index = (LCD_WIDTH * page + x);
  uint8_t bit_position = (y % 8);
  if (value == 0) {
    frameBuffer[byte_index] &= ~(1 << bit_position);  
  } else {
    frameBuffer[byte_index] |= (1 << bit_position);
  }
  //lcd_display(); // Sends the entire buffer after every pixel draw, VERY SLOW!
}


void lcd_flip(uint8_t horizontaly, uint8_t verticaly) {
  if (horizontaly > 1 || verticaly > 1) {
    printf("Error: Flipping values can only be 0 or 1\n"); 
    return;
  }
  send_command((horizontaly) ? comInvDirection : comDirection);
  send_command((verticaly) ? segInvDirection : segDirection);
  lcd_display();
}


void lcd_shift_horizontaly(uint8_t shift_amount) {
  if (shift_amount > 63) {
    printf("Warning: You cannot shift horizontaly more than 63 pixels.\n");
    shift_amount = 63;
  }
  send_command(setStartLine | shift_amount);
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


static pwmConfig pwm = {
  .slice_num = 69, // (69 if doesn't exist)
  .wrapping_point = 0, 
  .pin = 15, 
};

void lcd_enable_pwm_brightness(uint8_t pin, uint8_t pwm_frequency) {
  pwm.pin = pin;
  gpio_set_function(pwm.pin, GPIO_FUNC_PWM);

  pwm_config config = pwm_get_default_config();

  uint16_t sys_frequency = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS) / 1000;
  pwm.wrapping_point = (1000000 / pwm_frequency) / (1000 / sys_frequency);
  pwm_config_set_wrap(&config, pwm.wrapping_point);
  
  pwm.slice_num = pwm_gpio_to_slice_num(pwm.pin);
  pwm_init(pwm.slice_num, &config, false);
}


void lcd_set_brightness(uint8_t duty_cycle) {
  if (pwm.slice_num == 69) {
    printf("Error: You need to call lcd_enable_pwm_brigthness() first!\n");
    return;
  }
  if (duty_cycle > 100) {
    duty_cycle = 100;
    printf("Warning: Brightness cannot exceed 100%, defaulted back to 100%.\n");
  }
  if (duty_cycle == 0) {
    pwm_set_enabled(pwm.slice_num, false);
    return;
  }

  // "Duty cycle / 100" converts % to decimal, so we can multiply it with wrapping point
  // BUT here we multiply duty cycle first and then convert, thus avoiding floating-point operations
  uint16_t time_ON = (duty_cycle * pwm.wrapping_point) / 100;

  uint channel = pwm_gpio_to_channel(pwm.pin);
  pwm_set_chan_level(pwm.slice_num, channel, time_ON);

  pwm_set_enabled(pwm.slice_num, true);
}


void lcd_set_contrast(uint8_t RR_value, uint8_t EV_value) {
  if (EV_value > 63) {EV_value = 63;}
  if (RR_value > 7) {RR_value = 7;}

  send_command(regRatio | RR_value); // 0x04 recommended
  send_command(EVmode);
  send_command(EVset | EV_value); // 0x1F recommended
}


static inline void send_command(uint8_t command) {
  gpio_put(spi.CS, 0);
  spi_write_blocking(spi.ID, &command, 1);
  gpio_put(spi.CS, 1);
}


static inline void send_data(uint8_t data) {
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
  sleep_us(1);
}


void lcd_init() {
  lcd_reset();

  send_command(displayOFF);
  send_command(selectBias);
  send_command(segDirection);
  send_command(comInvDirection);
  send_command(powerControl);
  sleep_us(50);

  send_command(regRatio); 
  send_command(EVmode);
  send_command(EVset);

  send_command(setStartLine);
  send_command(displayON);
  send_command(allPixelsNormal);
}


void lcd_spi_init(
    spi_inst_t* spi_id, 
    uint8_t MOSI,
    uint8_t SCLK,
    uint8_t DC,
    uint8_t CS,
    uint8_t RST,
    uint16_t frequency) {

  if (frequency > frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS)) {
    printf("Error: Specified %d kHz is higher than system frequency!\n", frequency);
    return;
  } else if (frequency > 20000) {
    printf("Warning: Specified %d kHz SCLK is higher than 20MHz stated in the datasheet at 3.3V, 25°C.\n", frequency);
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
    SPI_CPOL_0,    // SCLK 0V at idle
    SPI_CPHA_0,    // Sampling edge (Rising in this case)
    SPI_MSB_FIRST  // Most significant bit first
  );
}
