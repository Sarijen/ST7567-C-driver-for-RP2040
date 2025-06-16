#include "ST7567.h"

static spi_config_t spi;

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define PAGE_COUNT 8

uint8_t frameBuffer[PAGE_COUNT * LCD_WIDTH]; // Contains 1024 Bytes, 128 Bytes per page

// Commands defined in binary for clear understanding
// CONFIGURATION
static const uint8_t EVmode =           0b10000001;
static const uint8_t EVset =            0b00000000;
static const uint8_t regRatio =         0b00100000;
static const uint8_t selectBias =       0b10100011;
static const uint8_t powerControl =     0b00101000;

static const uint8_t displayOFF =       0b10101110;
static const uint8_t displayON =        displayOFF | 0x01;

// PIXEL DATA MANIPULATION
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
static const uint8_t softwareReset =    0b11100010;



void lcd_draw_string(uint8_t x, uint8_t y, font_glyph* font, char string[]) {
  uint8_t string_length = strlen(string);
  uint8_t char_offset = font[0].width + 1;

  for (uint8_t current_char = 0; current_char < string_length; current_char++) {
    if (string[current_char] == ' ') {continue;}
    lcd_draw_character(x + (char_offset * current_char), y, font, string[current_char]);
  }
}


void lcd_draw_character(uint8_t x, uint8_t y, font_glyph* font, char character) {
  if (character == ' ') {return;}

  const uint8_t* bitmap_data;
  uint16_t matching_char = 0;

  while (1) { // Find our character in the font table
    if (character == font[matching_char].character) {
      bitmap_data = font[matching_char].bitmap_data;
      break;
    }

    // If given character is not in the table, use the 1st character
    // End of font table is indicated as a character with width = 0
    if (font[matching_char].width == 0) {
      bitmap_data = font[0].bitmap_data;
      break;
    }

    matching_char++;
  }

  // Draw our character
  uint16_t currentByte = 0;
  for (uint8_t i = x; i < x + font[matching_char].width; i++) {
    int currentY = y;
    for (uint8_t j = 0; j < (font[matching_char].height / 8); j++) {
      for (uint8_t bit = 0; bit < 8; bit++) {
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
  if (x > LCD_WIDTH || y > LCD_HEIGHT) {return;}
  if (invert > 1) {invert = 1;}

  uint8_t bytes_per_row = (width / 8);
  if ((width % 8) != 0) {bytes_per_row += 1;}

  for (int8_t xi = 0; xi < width; xi++) {
    for (int8_t yi = 0; yi < height; yi++) {

      uint16_t byte_index = (yi * bytes_per_row) + (xi / 8);
      uint8_t bit_index = 7 - (xi % 8);

      if (((image[byte_index] >> bit_index) & 1) == invert) {
        lcd_draw_pixel(xi + x , yi + y, 1); 
      }
    }
  }
}


void lcd_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t value) {
  if (x > LCD_WIDTH || y > LCD_HEIGHT) {return;}

  uint8_t dx = x + width;
  uint8_t dy = y + height;

  for (uint8_t i = x; i < dx; i++) { // We supplement x, y with i, j so they reset after looping
    for (uint8_t j = y; j < dy; j++) {
      lcd_draw_pixel(i, j, value);
    }            //  x  y
  }
}


void lcd_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t value) {
  if (x > LCD_WIDTH || y > LCD_HEIGHT) {return;}

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


void lcd_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t value) {
  if (x1 > LCD_WIDTH || y1 > LCD_HEIGHT) {return;}

  // Bresenham's algorithm used
  uint8_t dx = abs(x2 - x1); 
  uint8_t dy = abs(y2 - y1);
  int8_t sx = (x1 < x2) ? 1 : -1;
  int8_t sy = (y1 < y2) ? 1 : -1;
  int16_t error = dx - dy;

  while (x1 != x2 || y1 != y2) {
    lcd_draw_pixel(x1, y1, value);

    int16_t e2 = 2 * error;
    if (e2 > -dy) {
      error -= dy;
      x1 += sx;
    }

    if (e2 < dx) {
      error += dx;
      y1 += sy;
    }
  }
}


void lcd_draw_pixel(uint8_t x, uint8_t y, uint8_t value) {
  if (x > LCD_WIDTH || y > LCD_HEIGHT) {return;}

  uint8_t page = y / 8; // Pages are 8-bits high
  uint16_t byte_index = (LCD_WIDTH * page) + x;
  uint8_t bit_position = (y % 8);

  if (value == 0) {
    frameBuffer[byte_index] &= ~(1 << bit_position);  
  } else {
    frameBuffer[byte_index] |= (1 << bit_position);
  }

  //lcd_display(); // Sends the entire buffer after every pixel draw, VERY SLOW!
}


void lcd_flip(uint8_t horizontally, uint8_t vertically) {
  if (horizontally > 1 || vertically > 1) {
    printf("[ST7567] Error: Flipping values can only be 0 or 1\n"); 
    return;
  }

  send_command((horizontally) ? comInvDirection : comDirection);
  send_command((vertically) ? segInvDirection : segDirection);
  lcd_display();
}


void lcd_shift_horizontally(uint8_t shift_amount) {
  if (shift_amount > 63) {
    printf("[ST7567] Warning: You cannot shift horizontally more than 63 pixels.\n");
    shift_amount = 63;
  }

  send_command(setStartLine | shift_amount);
}


void lcd_toggle_invert(void) {
  if (displayInverted) {
    send_command(invertDisplay & ~1); // Flips the last bit
    displayInverted = 0;
  } else {
    send_command(invertDisplay);
    displayInverted = 1;
  }
}


// Loops through every byte in every page from the framebuffer and sends it
void lcd_display(void) {
  for (uint8_t currentPage = 0; currentPage < PAGE_COUNT; currentPage++) {
    send_command(setPageAddr | currentPage);
    send_command(setColAddrH); // Resets the Column Address for every page
    send_command(setColAddrL);

    uint16_t currentPageIndex = currentPage * LCD_WIDTH;
    uint16_t byte;

    for (byte = currentPageIndex; byte < currentPageIndex + LCD_WIDTH; byte++) {
      send_data(frameBuffer[byte]);
    }
  }
}


// Fills the buffer on MCU with 0s
void lcd_clear_buffer(void) {
  uint16_t frameBufferSize = LCD_WIDTH * PAGE_COUNT;

  for (uint16_t byte = 0; byte < frameBufferSize; byte++) {
    frameBuffer[byte] = 0x00;
  }
}


// Fills the screen with 0s
void lcd_clear_screen(void) {
  for (uint8_t currentPage = 0; currentPage < PAGE_COUNT; currentPage++) {
    send_command(setPageAddr | currentPage);
    send_command(setColAddrH); 
    send_command(setColAddrL); 

    for (uint8_t i = 0; i < LCD_WIDTH; i++) { 
      send_data(0x00);
    }
  }
}


static pwm_config_t pwm = {
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
    printf("[ST7567] Error: You need to call lcd_enable_pwm_brigthness() first!\n");
    return;
  }

  if (duty_cycle > 100) {
    duty_cycle = 100;
    printf("[ST7567] Warning: Brightness cannot exceed 100%, defaulted back to 100%.\n");
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


void lcd_software_reset(void) {
  send_command(softwareReset);
  sleep_us(1);
}


void lcd_hardware_reset(void) {
  gpio_put(spi.RST, 0);
  sleep_us(2);
  gpio_put(spi.RST, 1);
  sleep_us(1);
}


void lcd_init(void) {
  lcd_hardware_reset();

  send_command(displayOFF);
  send_command(selectBias);
  lcd_flip(1, 0);
  send_command(powerControl | 0b111);
  sleep_us(50);

  lcd_set_contrast(0x04, 0x1F);
  send_command(setStartLine);
  send_command(displayON);
  send_command(allPixelsNormal);
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


void lcd_spi_init(
    spi_inst_t* spi_id, 
    uint8_t MOSI,
    uint8_t SCLK,
    uint8_t DC,
    uint8_t CS,
    uint8_t RST,
    uint16_t spi_frequency) {

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

  if (spi_frequency > 20000) {
    printf("[ST7567] Warning: Specified SPI freq higher than 20MHz stated in the datasheet at 3.3V, 25°C.\n", spi_frequency);
  }

  uint32_t real_spi_frequency = spi_init(spi.ID, 1000 * spi_frequency);
  if (spi_frequency > frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS)) {
    printf("[ST7567] Warning: Specified SPI freq higher than sys freq.");
    printf("[ST7567] SPI freq set to %d kHz\n", real_spi_frequency / 1000); 
  }

  gpio_set_function(spi.MOSI, GPIO_FUNC_SPI);
  gpio_set_function(spi.SCLK, GPIO_FUNC_SPI);
  spi_set_format(spi.ID,
    8,             // 8 bits per transfer
    SPI_CPOL_0,    // SCLK 0V at idle
    SPI_CPHA_0,    // Sampling edge (Rising in this case)
    SPI_MSB_FIRST  // Most significant bit first
  );
}
